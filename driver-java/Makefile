# Flags
JFLAGS=-g

# Compiler (creates .class files)
JAVAC=javac

# Files to compile
sources=$(wildcard *.java)
classes=$(sources:.java=.class)

JARTARGET=networktablet.jar
JAR=jar
JARFLAGS=cmvf
JARMANIFEST=MANIFEST.MF

all=networktablet

networktablet: $(classes)

clean:
	rm --force *.class

%.class: %.java
	$(JAVAC) $<

jar: $(classes)
	$(JAR) $(JARFLAGS) $(JARMANIFEST) $(JARTARGET) $(classes)

