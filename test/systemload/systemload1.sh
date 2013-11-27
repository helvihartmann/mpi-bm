/* ~/mpich-install/mpich-3.0.4/myfiles/alltoall/example202.c
21.10.2013
class Mpi contains all MPi related functions
designed to send a lot of data between processes even more than 1GB

! This program shows how to use MPI_Alltoall.  Each processor
! send/rec a different created number*/

//#define mpi_root 0

int main(int argc,char *argv[]){
int *scounts;
//int *rcounts;
//int size, rank;
// uint64_t nsize;
size_t ncounts;
int systemload;

// read in data to send from console (default =1MB)
if (argc > 1){
ncounts = atoll(argv[1]);
}
else{
ncounts = 1000000;
}

systemload = getloadavg(double loadavg[], int nelem);
printf("%d\n",systemload);
//Allocates a block of size bytes of memory, returning a pointer to the beginning of the block
scounts= new int [ncounts];
//rcounts= new int [nsize];
for(size_t i=0; i<ncounts; i++){
scounts[i]=0;
}

printf("%ld Byte are allocated\n",ncounts);


//Finalize
delete [] scounts;
//delete [] rcounts;
}
