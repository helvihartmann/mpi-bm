#include "results.h"

Results::Results(int rank_, int statisticaliteration_, int numberofpackages_, MPI_Comm communicators_comm_, int size_) :
    rank(rank_),
    statisticaliteration(statisticaliteration_),
    numberofpackages(numberofpackages_),
    package_vector(numberofpackages),
    innerRuntimeIterations_vector(numberofpackages),
    totaldatasent_vector(numberofpackages),
    time(statisticaliteration * numberofpackages),
    summe(numberofpackages),
    communicators_comm(communicators_comm_),
    size(size_)
{
}

void Results::setvectors(int idx_outeriter_, size_t idx_numberofpackages, size_t innerRuntimeIterations, size_t packagesize_tmp, int numberofRemoteranks_, double time_){
    idx_outeriter = idx_outeriter_;
    numberofRemoteranks = numberofRemoteranks_;
    package_vector.at(idx_numberofpackages) = packagesize_tmp;
    innerRuntimeIterations_vector.at(idx_numberofpackages)=innerRuntimeIterations;
    
    totaldatasent_vector.at(idx_numberofpackages)=packagesize_tmp*innerRuntimeIterations*numberofRemoteranks;
    int index = idx_outeriter*numberofpackages+idx_numberofpackages;
    time.at(index) = time_;
    summe.at(idx_numberofpackages) += time_;
}


void Results::printstatisticaliteration(){
    std::cout << "# printing " << idx_outeriter << ". statistical iteration" << std::endl;
    for(int idx_numberofpackages = 0; idx_numberofpackages < numberofpackages; idx_numberofpackages++){
        int index = idx_outeriter*numberofpackages+idx_numberofpackages;
        std::cout<<totaldatasent_vector.at(idx_numberofpackages)<<" "<<innerRuntimeIterations_vector.at(idx_numberofpackages)<<" "<<package_vector.at(idx_numberofpackages)<<" " << time.at(index) <<" - "<< (totaldatasent_vector.at(idx_numberofpackages)/time.at(index))/1000000 << " - " << rank <<  " -  ";
        
        std::cout << " " << std::endl;
    }
    std::cout  << "\n\n" << std::endl;
}

void Results::calculate(){
    std::cout << "# process " << rank << " printing" << std::endl;
    for(int z=0;z<numberofpackages;z++){
        double send_mean=summe.at(z)/(statisticaliteration);
        double diff = 0;
        for (int m=0;m<statisticaliteration;m++){
            int index = m*numberofpackages+z;
            diff+= (send_mean - time.at(index))*(send_mean - time.at(index));
        }
        double rate =(totaldatasent_vector.at(z)/send_mean)/1000000;//(1<<20);
        double send_vartime = diff/(statisticaliteration);
        double send_stdtime = sqrt(send_vartime);
        double send_std=(send_stdtime/send_mean)*rate;
        
        std::cout<<totaldatasent_vector.at(z)<<" "<<innerRuntimeIterations_vector.at(z)<<" "<<package_vector.at(z)<<" "<<send_mean<<" "<<send_stdtime<<" "<<rate<< " " << send_std << " " << rank << std::endl;
    }
}


void Results::outputiteration(unsigned int m){
    MPI_Barrier(communicators_comm);
    for (int i=0; i<size; i++) {
        if (rank == i){
            if (rank == 0){
                cout << m << ". iteration-------------------" << endl;
                
            }
            printstatisticaliteration();
            sleep(2);
        }
        MPI_Barrier(communicators_comm);
    }
}

void Results::outputfinal(int commflag){
    for (int i=0; i<size; i++) {
        if (rank == i){
            if(rank == 0){
                cout << "# processes " << size << endl;
            }
            
            if (commflag == 0){
                cout << "#----------------------- SENDER ---------------------------" << endl;
                cout << "# totaldatasent repeats  packagesize time [us] std sendbandwidth [MB/s] std \n" << endl;
            }
            
            else {
                cout << "#--- RECEIVER ----------------------------" << endl;
            }
            
            calculate();
            cout << "\n\n" << endl;
            sleep(5);
        }
        MPI_Barrier(communicators_comm);
    }
}