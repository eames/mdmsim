from sys import argv
from numpy import *
from matplotlib.pyplot import *

#matplotlib.style.use('seaborn-ticks')
fig,ax=subplots(1,1,figsize=(5,5*.75))     
cc = rcParams['axes.prop_cycle'].by_key()['color']


fdata = 'results1.dat' if len(argv) <= 1 else argv[1]
print("reading from",fdata)

thi,E,theta,W1,W2,W3,W4=genfromtxt(fdata,skip_header=1).T
zpos=array([0.,15.1,16.3+15.1,16.3*2+15.1])


tanTheta,W1pos = [[tan(theta[0]/1000)]], [[W1[0]]]
evals = [E[0]]
j = 0
for i in range(1,len(E)):
    if(E[i] == evals[-1]):
        tanTheta[-1].append(tan(theta[i]/1000))
        W1pos[-1].append(W1[i])
    else:
        evals.append(E[i])
        tanTheta.append([tan(theta[i]/1000)])
        W1pos.append([W1[i]])

foci = empty(len(W1))
for i in range(len(W1)):
    which_e = 0
    for j in range(len(evals)):
        if abs(evals[j] - E[i]) < 1e-3:
            which_e = j
    plot([zpos[0],zpos[-1]],[W1[i],W4[i]],ls='-',color=cc[which_e])
yl = ylim()
for i in range(4):
    plot(ones(2)*zpos[i],yl,ls='--',color=cc[0])

focus = 0
for i in range(len(evals)):
    slp,off = polyfit(tanTheta[i], W1pos[i], 1)
    focus = -slp
    xfp = array(W1pos[i]) + focus*array(tanTheta[i])
    plot(focus, mean(xfp), ls='none', marker='o', label="%.0f MeV: %.2f cm"%(evals[i],focus))
    print("Energy = %.1f MeV, FP = %.2f cm downstream wire 1" % (evals[i], focus))

theLoc = 'upper right' if focus < 22.5 else 'upper left'
legend(loc=theLoc)
ylim(yl)
xl = xlim()
xticks(arange(0,55,5))
minorticks_on()
xlabel('Distance Downstream of Wire 1 [cm]')
ylabel('X Position [cm]')
#ylim(-18,18)
if len (argv) > 2:
    text(xl[0], yl[1] + 0.05*(yl[1]-yl[0]), argv[2])
fig.set_tight_layout(True)
show()

