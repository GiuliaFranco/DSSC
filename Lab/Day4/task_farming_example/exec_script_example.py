import os
myid = int( os.environ['OMPI_COMM_WORLD_RANK'] )

if( myid % 2 == 0):
    print("\nI am %d and I am an even process\n" % myid);
else:
    print("\nI am %d and I am an odd process\n" % myid);
