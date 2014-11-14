import sys


team = sys.argv[1]
totalScore = 0
winScore = 0
wins = 0.0
count = 0

f = open(team, "r").readlines()
open(team, "r").close()

for line in f:
    line = line.split(",")
    i = 0
    while i < len(line):
        if "11-11" in line[0] and (line[i]==team.replace("_"," ") \
                                   or line[i]==team):
            score = float(line[i+1])
            if line[5] == "W":
                winScore+= score
                wins+= 1
            totalScore+= score
            count+=1
            break
        i+=1

print "team:", team
print "wins / count:", wins, count
print "win rate (wins/count):", 100*wins/count
print "average score:", totalScore/count
print "average score out of winnings:", winScore/wins, 
