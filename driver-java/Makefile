# Flags
JFLAGS=-g

# Compiler (creates .class files)
JAVAC=javac

# Files to compile
sources=$(wildcard *.java)
classes=$(sources:.java=.class)

# Jar name
JARTARGET=networktablet.jar

# Self explanatory
JAR=jar

# Jar flags
JARFLAGS=cmvf

# Using a manifest file to assign the main java class
JARMANIFEST=MANIFEST.MF

#all=networktablet

# create jar file (compiles everything)
jar: $(classes)
	$(JAR) $(JARFLAGS) $(JARMANIFEST) $(JARTARGET) $(classes)

# just create the class files
networktablet: $(classes)

%.class: %.java
	$(JAVAC) $<

# clear files
clean:
	rm --force *.class
