
import threading
import time
import os
import sys
import subprocess

repo_path = 'C:\\Users\\FHales\\Documents\\MiniCityTest\\MiniCity'
previousCommit = ""
latestCommit = ""

#Log file variables
timeFound = ""
buildPassRelease = False
buildPassDebug = False
gitPass = False
unitTestsResults = []
otherLogInfo = []

def RunBatchFile(args):
	#args = ['VS_Build.bat']
	try:
		result = subprocess.run(args, shell=True, capture_output=True, check=True, cwd = repo_path)
		return [ True, result.stdout.decode("ascii")]
	except subprocess.CalledProcessError as e:
		print("Batch Command Error: \n", e.stdout.decode("ascii"))
		return [False, e.stderr.decode("ascii")]

def RunGitCommand(args):
	try:
		result = subprocess.run(args, capture_output=True, check=True, cwd = repo_path)
		return [ True, result.stdout.decode("ascii")]
	except subprocess.CalledProcessError as e:
		print("Git Command Error: \n", e.stderr.decode("ascii"))
		return [False, e.stderr.decode("ascii")]
		

def CheckIfGitChange():
	global repo_path
	global previousCommit
	global latestCommit

	result = RunGitCommand(["git", "log", "-n", "1", "--pretty=format:\"%H\"", "main"])

	if(result[0]):
		latestCommit = result[1]
		if(latestCommit != previousCommit):
			previousCommit = latestCommit
			return True
		else:
			return False
	else:
		print(result[1])

def RunBatchCommands():
	result = RunBatchFile(['VS_Build_Debug.bat'])
	if(result[0]):
		buildPassDebug = True
	else:
		buildPassDebug = False

	otherLogInfo.append(result[1])

	result = RunBatchFile(['VS_Build_Release.bat'])
	if(result[0]):
		buildPassRelease = True
	else:
		buildPassRelease = False
	
	otherLogInfo.append(result[1])

def RunGitCommands():
	gitPass = True

	result = RunGitCommand(["git", "clean", "-f"])
	if(not result[0]):
		gitPass = False
		otherLogInfo.append(result[1])
	
	result = RunGitCommand(["git", "reset"])
	if(not result[0]):
		gitPass = False
		otherLogInfo.append(result[1])

	result = RunGitCommand(["git", "pull"])
	if(not result[0]):
		gitPass = False
		otherLogInfo.append(result[1])


def PollForChanges():
	if(CheckIfGitChange()):
		print("New git commit!")
		timeFound = time.ctime()
		filename = timeFound + "_" + latestCommit + ".log"
		otherLogInfo.clear()
		unitTestsResults.clear()

		with open("logs\\" + filename, 'w') as file:
			file.write("Commit: " + latestCommit + "\n")
			file.write("Time: " + timeFound + "\n")

			RunGitCommands()
			RunBatchCommands()

			file.write("Git status: " + gitPass + "\n")
			file.write("Release build status: " + buildPassRelease + "\n")
			file.write("Debug build status: " + buildPassDebug + "\n")

			for otherInfo in otherLogInfo:
				file.write(otherInfo + "\n")
				

def RunForever():
	WAIT_TIME_SECONDS = 5
	ticker = threading.Event()
	while not ticker.wait(WAIT_TIME_SECONDS):
		PollForChanges()

PollForChanges()
# # RunForever()