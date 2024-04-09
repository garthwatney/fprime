#!/bin/env python3
# --------------------------------------------------------------------------------------
# Filename: graph.py
#
# Input: stdin (flow of telemetry channels that contains the updated state)
# Starts a Python Flask that updates a state machine image in real time and
# renders it on localhost:8080
# This program assumes that the plantuml server is local and running on 
# localhost:8080.  See the Notes in this directory that explains how to start up
# the plantuml server.
# The stdin is from the F` telemetry channel command line
#
# Usage:
# If tlm.py is filtering on telemetry channels.
#
# ./tlm.py | graph.py
#
# This program is currently hard coded to use LedSm.plantuml.  To be changed
#
# --------------------------------------------------------------------------------------


import re
from plantuml import PlantUML
from flask import Flask, Response
import sys
import threading
from queue import Queue

global originalContent

PLANTUML_FILE = "../Svc/FPManager/FPManagerSm.plantuml"

# --------------------------------------------------------------------------------------
# Function: modify_content
#
# Description:
# The `modify_content` function is designed to update the PlantUML content stored
# in the global variable `originalContent`. It takes a single argument, `state`,
# which specifies the state within the PlantUML diagram that needs to be highlighted.
# The function constructs a regular expression pattern to identify the exact
# occurrence of the specified state in the PlantUML content. It then uses this
# pattern to replace the original state declaration with a new one that includes
# a color directive (#gold) to highlight the state in the generated diagram.
# The updated content is returned, with the specified state highlighted in gold,
# without altering the original content for subsequent operations.
# --------------------------------------------------------------------------------------

def modify_content(state):
    global originalContent

    pattern = r"\bstate " + re.escape(state) + r"\b"
    return re.sub(pattern, f"state {state} #gold", originalContent)

# --------------------------------------------------------------------------------------
# Function: generate_diagram_from_string
#
# Description:
# The `generate_diagram_from_string` function takes a string `content` as input,
# which should contain PlantUML diagram code. It sends this content to the PlantUML
# server specified by `plantuml_url` to generate a diagram. The diagram is returned
# as raw PNG data. If the diagram generation is successful, the PNG data is returned.
# If an exception occurs during the generation process, the function captures the
# exception, prints an error message, and returns None to indicate the failure.
# This function is essential for converting PlantUML code into a visual diagram
# in PNG format, which can be served or stored as needed.
# --------------------------------------------------------------------------------------

def generate_diagram_from_string(content):
    plantuml_url = 'http://localhost:8080/img/'
    plantuml = PlantUML(plantuml_url)
    try:
        rawpng = plantuml.processes(content)
        return rawpng  # Return the raw PNG data
    except Exception as e:
        # Updated error handling
        print(f"An error occurred while generating the diagram: {str(e)}")
        return None  # Return None in case of an error

# --------------------------------------------------------------------------------------
# Function get_state
#
# Description:
# The `get_state` function continuously reads lines from the standard input (sys.stdin).
# It is designed to listen for state changes, which are expected to be conveyed through
# the lines it reads. Each line is checked for the presence of the string "[INFO]".
# If this string is found, the line is ignored, as it's considered informational and
# not indicative of a state change. For lines not containing "[INFO]", the function
# assumes the last word in the line represents a new state. This state is extracted
# and enqueued onto `state_queue`, a shared queue where state information is stored.
# This function is typically run in a separate thread, allowing the program to
# asynchronously monitor and react to state changes communicated via standard input.
# --------------------------------------------------------------------------------------

def get_state():
    for line in sys.stdin:
        if "[INFO]" in line:
            continue
        state = line.split()[-1]
        state_queue.put(state)   

# --------------------------------------------------------------------------------------
# Function start_flask
#
# Description:
# The `start_flask` function is responsible for initiating the Flask web server. It
# first logs a message indicating that the Flask thread is starting. The function then
# calls `app.run(port=8080)` to start the Flask application, listening on port 8080.
# This function is intended to be run in a separate thread, allowing the Flask server
# to operate concurrently with other parts of the application, such as the state
# monitoring system. The web server serves the endpoints defined elsewhere in the
# application, facilitating interactions like image generation and serving based on
# the application's current state.
# --------------------------------------------------------------------------------------

def start_flask():
    print(f'Starting thread start_flask')
    app.run(port=8081)

# --------------------------------------------------------------------------------------
# Function: image
# 
# Description:
# The `image` function is set up to serve images dynamically generated based on the
# current state of the application. It uses a global variable `png_data` to hold the
# binary data of the generated image. The function attempts to fetch the latest
# state from a queue in a non-blocking manner. If a new state is available, it
# modifies the content for the image generation and updates `png_data` with the new
# image. The function then returns a response containing the image data in `png_data`
# with a MIME type of 'image/png'. If there is no new state, the function returns
# the last successfully generated image, ensuring that there is always an image
# returned in the response.
# --------------------------------------------------------------------------------------
app = Flask(__name__)

@app.route('/image')
def image():
    global png_data
    try:

        # Retrieve the latest state available, discarding any unprocessed older states.
        while not state_queue.empty():
            state = state_queue.get(block=False)

        content = modify_content(state)
        png_data = generate_diagram_from_string(content)
    finally:
        return Response(png_data, mimetype='image/png')


# --------------------------------------------------------------------------------------
# Main
# 
# Description:
# Main Program Description:
# This script initializes a Flask application and a queue for managing state information.
# It reads the initial PlantUML content from "LedSm.plantuml" and generates an initial
# diagram. Two separate threads are created and started: one for monitoring state changes
# through standard input and another for running the Flask web server. The state monitoring
# thread listens for new state information and updates the state queue, which influences
# the diagram generation. Concurrently, the Flask server thread serves the dynamic image
# and other endpoints, allowing for real-time updates and interactions through a web interface.
# This setup enables the application to handle asynchronous updates and serve content
# simultaneously, facilitating a responsive and interactive user experience.
# --------------------------------------------------------------------------------------

state_queue = Queue()

if __name__ == '__main__':

    with open(PLANTUML_FILE, 'r') as file:
        originalContent = file.read()

    png_data = generate_diagram_from_string(originalContent)

    state_thread = threading.Thread(target=get_state)
    flask_thread = threading.Thread(target=start_flask)
    state_thread.start()
    flask_thread.start()
