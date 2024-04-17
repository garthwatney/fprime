BACK-END = fprime
NAMESPACE = Svc
MODEL = FPManagerSm

autocode:
	@printf "**** Running the state-machine autocoder ****\n"
	../../STARS/autocoder/Stars.py -backend $(BACK-END) -namespace $(NAMESPACE) -noImpl -model $(MODEL).plantuml
	../../STARS/autocoder/Stars.py -smbase
