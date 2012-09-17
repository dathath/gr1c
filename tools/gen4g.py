#!/usr/bin/env python


import sys


if len(sys.argv) != 5:
    print "Usage: %s xvar yvar H W" % sys.argv[0]
    exit(1)

x_vname = sys.argv[1]
y_vname = sys.argv[2]
height = int(sys.argv[3])
width = int(sys.argv[4])

# Assume that width and height are at least 3.


# Transitions
print "& [](("+x_vname+"=0) -> ("+x_vname+"'=0 | "+x_vname+"'=1))"
print "\n".join(["& [](("+x_vname+"="+str(k)+") -> ("+x_vname+"'="+str(k-1)+" | "+x_vname+"'="+str(k)+" | "+x_vname+"'="+str(k+1)+"))" for k in range(1,width-1)])
print "& [](("+x_vname+"="+str(width-1)+") -> ("+x_vname+"'="+str(width-2)+" | "+x_vname+"'="+str(width-1)+"))"
print "& [](("+y_vname+"=0) -> ("+y_vname+"'=0 | "+y_vname+"'=1))"
print "\n".join(["& [](("+y_vname+"="+str(k)+") -> ("+y_vname+"'="+str(k-1)+" | "+y_vname+"'="+str(k)+" | "+y_vname+"'="+str(k+1)+"))" for k in range(1,height-1)])
print "& [](("+y_vname+"="+str(height-1)+") -> ("+y_vname+"'="+str(height-2)+" | "+y_vname+"'="+str(height-1)+"))"

# 4-connected
print "& []("+" | ".join(["("+x_vname+"="+str(k)+" & "+x_vname+"'="+str(k)+")" for k in range(width)])+" | "+" | ".join(["("+y_vname+"="+str(k)+" & "+y_vname+"'="+str(k)+")" for k in range(height)])+")"