import math

PI       = 3.141592
FREQ     = 200.0
OMEGA    = FREQ*PI*2.0

PHI_STEPS = 1000
V_AMP     = 2047

T  = 1./FREQ
dt = T /PHI_STEPS

def print_sine(s):
  print ( "#ifndef _Waveforms_h_" )
  print ( "#define _Waveforms_h_" )
  print ( "" )
  print ( "#define PI {:.6f}".format(PI) )
  print ( "" )
  print ( "#define FREQ {:d}".format(int(FREQ)) )
  print ( "#define PHI_STEPS {:d}".format(PHI_STEPS) )
  print ( "#define V_AMP {:d}".format(V_AMP) )
  print ( "" )
  print ( "#define OMEGA (PI*FREQ*2.0)" )
  print ( "#define T (1000000/FREQ) // microseconds" )
  print ( "#define DT (T/PHI_STEPS) // microseconds" )
  print ( "" )

  #print ('min s: %s' % min(s) )
  #print ('max s: %s' % max(s) )  
  print ("static int sine_wave[] = {")
  s_rounded = [hex(int(round(elem))) for elem in s]
  #print str(s)
  #print str(s_rounded)
  chunks = [ s_rounded[x:x+10] for x in range(0, len(s_rounded), 10) ]
  for chunk in chunks:
    chunk_str = [str(elem) for elem in chunk]
    chunk_form = '  ' + ', '.join(chunk_str) + ','
    print (chunk_form)
  #print ( ' ,'.join( )  )
  #for chunk in chunks:
  #  print (chunk)  
  #print ( ' ,'.join(str(s)) )
  print ("};")
  print ( "#endif" )

def print_excel(s):
  for phi in range(0, PHI_STEPS):
    t = float(phi)/PHI_STEPS * T
    v = s[phi]
    t1, v1 = t, int(round(v))
    #print( "{:f}\t{:f}".format(t, v) )
    print( "{:f}\t{:d}".format(t1, v1) )


sine_list = []

for phi in range(0, PHI_STEPS):
  t = float(phi)/PHI_STEPS * T
  #print ('phi = %s ' % phi)
  #print ('t = %s '   % t)
  s = V_AMP * math.sin(OMEGA * t) + V_AMP
  sine_list.append(s)

print_sine(sine_list)
#print_excel(sine_list)




