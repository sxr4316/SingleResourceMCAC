#!/usr/bin/python
# Tight Squad SABRE (Squad Automatic Bug Reporting Environment)
# Written by Matt Zachary
# Rev'd by Connor Goldberg
# For ee720 - Design of Digital Systems
# 3/30/2016
# Gmail Credentials: Username: squad.sabre, Password: hunter22

import argparse
import email.utils
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import os
import random
import re
import smtplib
import sys

PREVIEW_BLACKLIST = ["--=", "Content-Type:", "MIME-Version:", "Content-Transfer-Encoding"]

clearScreen = lambda: os.system("clear")

def getTerminalSize():
	env = os.environ
	def ioctl_GWINSZ(fd):
		try:
			import fcntl, termios, struct
			cr = struct.unpack('hh', fcntl.ioctl(fd, termios.TIOCGWINSZ,
		'1234'))
		except:
			return
		return cr
	cr = ioctl_GWINSZ(0) or ioctl_GWINSZ(1) or ioctl_GWINSZ(2)
	if not cr:
		try:
			fd = os.open(os.ctermid(), os.O_RDONLY)
			cr = ioctl_GWINSZ(fd)
			os.close(fd)
		except:
			pass
	if not cr:
		cr = (env.get('LINES', 25), env.get('COLUMNS', 80))
	return int(cr[1]), int(cr[0])

#Performs the SMTP connection to send the message
def sendEmail (addrList, subject, message, force, files):
	#Connection Info
	servername = "smtp.gmail.com"
	name = "SABRE"
	username = "squad.sabre@gmail.com"
	password = "hunter22"
	signature = "This message was automatically generated by the Squad Automatic Bug Report Experiment a.k.a. SABRE (trademarked). Please do not reply to this message."
	if subject == None:
		subject = "SABRE Notification"
	else:
		subject = " ".join(subject)

	#Form the message
	msg = MIMEMultipart()
	msg.set_unixfrom('author')
	msg['To'] = ", ".join(addrList)
	msg['From'] = email.utils.formataddr((name, username))
	msg['Subject'] = subject

	msg.attach(MIMEText(message + "\n\n-----\n%s" % signature, 'plain')) # Attach body

	clearScreen()

	validAttachments = []
	for fileName in files:
		if os.path.isfile(fileName):
			validAttachments.append(fileName)
		else:
			print "WARNING: Could not find file: %s" % fileName

	print "To: " + msg['To']
	linesToPreview = []
	for line in msg.as_string().split("\n")[3:]:
		addLine = True
		for blackString in PREVIEW_BLACKLIST:
			if blackString in line:
				addLine = False
		if addLine:
			linesToPreview.append(line)

	preview = "\n".join(linesToPreview)
	print preview
	print "Attachments: %s" % ", ".join([os.path.basename(a) for a in validAttachments])
	print getTerminalSize()[0]*'-' + '\n'
	if not force:
		num = random.randint(100,999)
		confirmation = raw_input("Please review the message preview above. If you are sure you would like " + \
									"to send this email please enter the following number: %s\n> " % num)
		if confirmation != str(num):
			clearScreen()
			print "ERROR: Wrong confirmation number. Please try again."
			sys.exit(1)

	print "Sending email"

	# Attach files
	for fileName in validAttachments:
		attachment = MIMEText(open(fileName).read())
		attachment.add_header('Content-Disposition','attachment',filename=os.path.basename(fileName))
		msg.attach(attachment)

	server = smtplib.SMTP(servername)
	
	#Send Stuff
	try:
		server.set_debuglevel(False)

		#Identify ourselves, prompting server for supported features
		server.ehlo()

		#If we can encrypt this session, do it
		if server.has_extn('STARTTLS'):
			server.starttls()
			server.ehlo() #Re-identify ourselves over TLS connection

		#Login and Send
		server.login(username, password)
		server.sendmail(username, addrList, msg.as_string())

		#All Done
		server.quit()
		clearScreen()
		print "Email sent successfully! Thank you for using SABRE."
	except Exception as e:
		server.quit()
		clearScreen()
		print "ERROR: Could not send email."
		print "Exception: %s" % e
		sys.exit(1)
	return

def main(args):
	addrList = args["address"]
	if args["body"] != None:
		body = " ".join(args["body"]).replace("\\n", '\n')
	else:
		body = ''

	files = []
	if args["file"] != None:
		files = args["file"]

	badAddresses = [a for a in addrList if not re.match(r"[^@]+@[^@]+\.[^@]+", a)]
	if badAddresses:
		print "ERROR: Found badly formatted email addresses: %s" % ", ".join(badAddresses)
		sys.exit(1)

	#Call the Send Mail Function
	sendEmail(addrList, args["subject"], body, args["force"], files)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Send a SABRE report!")
	req = parser.add_argument_group('required arguments')
	req.add_argument("-a", "--address", nargs='+', required=True, metavar="address", type=str, help="The addresses where to send the email")
	parser.add_argument("-b", "--body", nargs='+', required=False, metavar="body", type=str, help="The body of the email")
	parser.add_argument("-s", "--subject", nargs='+', required=False, metavar="subject", type=str, help="The subject of the email")
	parser.add_argument("-l", "--file", nargs='+', required=False, metavar="file", type=str, help="Files to attach")
	parser.add_argument("-f", "--force", action="store_true", required=False, help="Force send email. Use this carefully!")
	args = vars(parser.parse_args())
	main(args)