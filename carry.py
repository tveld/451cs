import sys

def gi(xi, yi):
	return xi,' AND ', yi

def pi(xi, yi):
	return xi,' OR ', yi

def ci(n):
	xi = int(n) - 1
	yi = int(n) - 1
	if(xi == -1):
		return ' ) '
	else:
		print gi(xi, yi) , ' OR ' , pi(xi, yi) , ' AND ( ' + ci(xi) + ")"

ci(sys.argv[1])
