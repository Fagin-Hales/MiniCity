
import threading
import time
import os
import sys
import subprocess

repo_path = 'C:\\Users\\FHales\\Documents\\MiniCityTest\\MiniCity'
previousCommit = ""
latestCommit = ""
latestCommitMessage = ""

#Log file variables
buildPassRelease = False
buildPassDebug = False
gitPass = False
testsPass = False
unitTestsResults = ""
otherLogInfo = []

def RunBatchFile(args):
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
		
def RunFile(args):
	try:
		result = subprocess.run(args[0], shell=True, capture_output=True, check=True, cwd = args[1])
		return [ True, result.stdout.decode("ascii")]
	except subprocess.CalledProcessError as e:
		print("Command Error: \n", e.stdout.decode("ascii"))
		return [False, e.stderr.decode("ascii")]

def CheckIfGitChange():
	global repo_path
	global previousCommit
	global latestCommit

	result = RunGitCommand(["git", "ls-remote", "origin", "HEAD"])

	if(result[0]):
		latestCommit = result[1]
		latestCommit = latestCommit.replace('\"', "").split('\t')[0]
		if(latestCommit != previousCommit):
			previousCommit = latestCommit
			return True
		else:
			return False
	else:
		print(result[1])

def RunBatchCommands():
	global buildPassDebug
	global buildPassRelease
	global otherLogInfo

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
	global gitPass
	global otherLogInfo

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
	global otherLogInfo
	global unitTestsResults
	global latestCommit
	global latestCommitMessage
	global gitPass
	global testsPass
	global buildPassDebug
	global buildPassRelease


	if(CheckIfGitChange()):
		print("New git commit!")
		timeFound = time.ctime()
		filename = timeFound + "_" + latestCommit + ".log"
		filename = filename.replace(':', '_')
		otherLogInfo.clear()
		
		RunGitCommands()
		RunBatchCommands()

		result = RunFile([["UnitTests.exe"],repo_path + "\\Game\\x64\\Release"]);
		if(not result[0]):
			testsPass = False
		else:
			testsPass = True
		otherLogInfo.append(result[1])

		result = RunGitCommand(["git", "log", "-n", "1", "--pretty=format:%s", latestCommit])
		if(not result[0]):
			gitPass = False
			otherLogInfo.append(result[1])
			latestCommit = ""
		else:
			latestCommitMessage = result[1]

		with open("commit_Logs\\" + filename, 'w') as file:
			file.write("Commit: " + latestCommit + "\n")
			file.write("Message: " + latestCommitMessage + "\n")
			file.write("Time: " + timeFound + "\n")
			file.write("Git status: " + str(gitPass) + "\n")
			file.write("Release build status: " + str(buildPassRelease) + "\n")
			file.write("Debug build status: " + str(buildPassDebug) + "\n")
			file.write("Unit tests status: " + str(testsPass) + "\n\n")

			file.write("Outputs: " + "\n")

			for otherInfo in otherLogInfo:
				file.write(otherInfo + "\n")
	else:
		print("No new git commit")

				

def RunForever():
	WAIT_TIME_SECONDS = 5
	ticker = threading.Event()
	while not ticker.wait(WAIT_TIME_SECONDS):
		PollForChanges()

#PollForChanges()
RunForever()