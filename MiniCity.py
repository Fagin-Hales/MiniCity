
import threading
import time
import os
import sys
import subprocess

repo_path = 'C:\\Users\\FHales\\Documents\\MiniCityTest\\MiniCity'
previousCommit = ""
latestCommit = ""

def RunBatchFile(args):
	#args = ['VS_Build.bat']
	try:
		result = subprocess.run(args, shell=True, capture_output=True, check=True, cwd = repo_path)
		return result.stdout.decode("ascii")
	except subprocess.CalledProcessError as e:
		print("Batch Command Error: \n", e.stdout.decode("ascii"))

def RunGitCommand(args):
	try:
		result = subprocess.run(args, capture_output=True, check=True, cwd = repo_path)
		return result.stdout.decode("ascii")
		#print(result.stdout.decode("ascii"))
	except subprocess.CalledProcessError as e:
		print("Git Command Error: \n", e.stderr.decode("ascii"))
		

def CheckIfGitChange():
	global repo_path
	global previousCommit
	global latestCommit

	latestCommit = RunGitCommand(["git", "log", "-n", "1", "--pretty=format:\"%H\"", "master"])
	#print(latestCommit)
	if(latestCommit != previousCommit):
		previousCommit = latestCommit
		return True
	else:
		return False

def RunBatchCommands():
	RunBatchFile(['VS_Build_Debug.bat'])
	RunBatchFile(['VS_Build_Release.bat'])

def RunGitCommands():
	RunGitCommand(["git", "clean", "-f"])
	RunGitCommand(["git", "reset"])
	RunGitCommand(["git", "pull"])


def PollForChanges():
	if(CheckIfGitChange()):
		print("New git commit!")
		print(time.ctime())
		RunGitCommands()
		RunBatchCommands()

def RunForever():
	WAIT_TIME_SECONDS = 5
	ticker = threading.Event()
	while not ticker.wait(WAIT_TIME_SECONDS):
		PollForChanges()

PollForChanges()
# # RunForever()