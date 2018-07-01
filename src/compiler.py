# -*- coding: utf-8 -*-
import sys

HELPMSG = "usage: python compiler.py [-d] [-h] <file>"\
		"\n"\
		"developed for python 3 but works for python 2 as well\n"\
		"\n"\
		"options\n"\
		"   -d      \tenables debug\n"\
		"   -h or -?\tprints this message\n"\
		"\n"\
		"made my janekx21"

DEBUG = False

def printDebug(string):
	if DEBUG:
		print(string)

useBytheCode = False

lastNumber = 0
allVars = {}

outcode = []

def ao(out):
	outcode.append(out)

def av(var):
	try:
		ao(str(allVars[var]))
	except KeyError:
		print("syntax error")
		print("{} is not defined".format(var))
		sys.exit(1)
	

def defVariable(lits):
	global lastNumber
	allVars[lits[0][1:]] = lastNumber
	lastNumber+=1

def defLabel(lits):
	allVars[lits[0][1:]] = programmCounter
	print("Die Zeit '{}' wird definiert.".format(lits[0][1:]))


articles = [ #wird zu &
	" ein ",
	" eine ",
	" der ",
	" das ",
	" die ",
	" den ",
	" dem ",
	" dieser ",
	" dieses ",
	" diese ",
	" einem ",
	" einen ",
	" einer ",
	"Die ",
	"Der ",
	"Das "
]
#lange zuerst

def nothing(args):pass

commands = {
	"Tom sieht & # mit & #.":"adding",
	"Tom sieht & $.":defVariable,
	"am $:":nothing,
	"Es wird $.":"jmp",
	"Tom sagt $.":"debugPrint",
	"Kennt Tom & #?":"testDefine",
	" & # ist jetzt $.":"setVar",
	"Tom listet alle Sachen auf die er kennt.":"listAllVars",
	"Ist # eigendlich #?":"compare",
	"Tom starrt aus dem Fenster.":"initWindow",
	"Die Welt wird von einem Meteor vernichtet.":"quit",
	"Tom denkt kurz nach.":"update",
	"Tom schaut in die Sonne.":"fillScreen",
	"Ich sag jetzt mal$":nothing,
	"Tom zeigt auf & #.":"printVar",
}

bytheCode = {
	"Tom sieht & # mit & #.":0,
	"Tom sieht & $.":defVariable,
	"am $:":nothing,
	"Es wird $.":1,
	"Tom sagt $.":2,
	"Kennt Tom & #?":3,
	" & # ist jetzt $.":4,
	"Tom listet alle Sachen auf die er kennt.":5,
	"Ist # eigendlich #?":6,
	"Tom starrt aus dem Fenster.":7,
	"Die Welt wird von einem Meteor vernichtet.":8,
	"Tom denkt kurz nach.":9,
	"Tom schaut in die Sonne.":10,
	"Ich sag jetzt mal$":nothing,
	"Tom zeigt auf & #.":11,
}

ncommands = {}
for k,v in commands.items():
	for article in articles:
		ncommands[k.replace(article," & ")] = v
commands = ncommands

def addCommand(command,lits):
	ao(command)
	for lit in lits:
		if(lit[0] == "V"):
			av(lit[1:])
		else:
			ao(lit[1:])
	ao("")

def addCommandBythe(command,lits):
	ao(command)
	for lit in lits:
		if(lit[0] == "V"):
			try:
				ao(allVars[lit[1:]])
			except KeyError:
				print("syntax error")
				print("{} is not defined".format(lit[1:]))
				sys.exit(1)
		else:
			ao(lit[1:])

def patternMatch(string,commands,failMSG):
	if len(string) == 0:return
	for pattern,function in commands.items():
		lits = testPattern(pattern,string)
		if lits!= False:
			printDebug("---match in p:{} and s:{} with literals:{}".format(pattern,string,lits))
			if commands[pattern] != None:
				if type(commands[pattern]) == str:
					addCommand(commands[pattern],lits)
				elif type(commands[pattern]) == int:
					addCommandBythe(commands[pattern],lits)
				else:
					commands[pattern](lits)
			return
		else:
			printDebug("---NO match in p:{} and s:{}".format(pattern,string))
	if failMSG:print("Den Satz:'{}' gibt es nicht.".format(string))



MAXLITERAL = 64

def testPattern(pattern,string):
	pattern+= "\0"
	string += "\0"

	literals = []
	stringIndex = 0
	x = 0

	#go throu all chars of the pattern
	while x < len(pattern):
		if pattern[x] == "$":
			target = pattern[x+1]#TODO error handling #not needet
			for y in range(MAXLITERAL):
				char = None
				try:
					char = string[stringIndex+y]
				except IndexError:
					return False
				if char:
					if char == target:
						literals.append("L"+string[stringIndex:stringIndex+y])
						stringIndex+=y
						x+=1
						break
		if pattern[x] == "#":
			target = pattern[x+1]#TODO error handling #not needet
			for y in range(MAXLITERAL):
				char = None
				try:
					char = string[stringIndex+y]
				except IndexError:
					return False
				if char:
					if char == target:
						literals.append("V"+string[stringIndex:stringIndex+y])
						stringIndex+=y
						x+=1
						break

		try:
			if pattern[x] != string[stringIndex]:
				return False
		except IndexError:
			return False
		stringIndex += 1
		x += 1

	return literals

programmCounter = 0
orginalCode = None
orginalLines = None
NWLines = None
replacedCode = None



def main(code):
	print("------------")
	print(code)
	print("------------\n")

	global programmCounter,orginalCode,orginalLines,replacedCode,NWLines
	orginalCode = code
	orginalLines= orginalCode.split("\n")

	#removing whitespace
	newlines = []
	for l in orginalLines:
		newlines.append(l.lstrip())
	code = "\n".join(newlines)
	NWLines = newlines

	#replacing articles with &
	for article in articles:
		code = code.replace(article," & ")
	replacedCode = code

	global programmCounter
	lines = code.split("\n")

	#defining all labels
	for i,line in enumerate(lines):
		programmCounter = i
		patternMatch(line,{"am $:":defLabel},False)

	#exectung programm
	programmCounter = 0
	print("Beginne das Programm.\n")
	while programmCounter < len(lines):
		line = lines[programmCounter]
		if not useBytheCode:
			patternMatch(line,commands,True)
		else:
			patternMatch(line,bytheCode,True)
		programmCounter+=1

	print("OUTCODE:\n--------")
	if useBytheCode:
		import struct
		with open("output.txt","wb") as f:
			bs = []
			for b in outcode:
				if type(b) == str:
					try:
						b = int(b)
					except:
						print("coult not parse {}".format(b))
						sys.exit(1)
				f.write(struct.pack("I",b))
		print(outcode)

	else:
		with open("output.txt","w") as f:
			f.write("\n".join(outcode))
		print("\n".join(outcode))

	print(allVars)

def tryArg(arg,test):
	try:
		if arg[1:] == test:
			return True
	except Exception as e:return False

if __name__ == "__main__":
	file = None
	for arg in sys.argv[1:]:
		if arg[0] == "-":

			if tryArg(arg,"d"):
				DEBUG = True
				print("DEBUG enabled")

			elif tryArg(arg,"h") or tryArg(arg,"?"):
				print(HELPMSG)
				sys.exit(0)

			elif tryArg(arg,"b"):
				useBytheCode = True

			else:
				print("option {} not known".format(arg))
				sys.exit(1)
			
		else:
			file = arg
	if file != None:
		with open(file,"r") as f:
			main(f.read())

	else:
		print(HELPMSG)