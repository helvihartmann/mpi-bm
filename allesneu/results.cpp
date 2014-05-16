#include "results.h"

Results::Results(int rank_, int statisticaliteration_, int numberofpackages_) :
  rank(rank_),
  statisticaliteration(statisticaliteration_),
  numberofpackages(numberofpackages_),
  package_vector(numberofpackages),
  innerRuntimeIterations_vector(numberofpackages),
  totaldatasent_vector(numberofpackages),
  time(statisticaliteration * numberofpackages),
  summe(numberofpackages)
{
    std::cout << "bla" << std::endl;
}

void Results::setvectors(int idx_outeriter, size_t idx_numberofpackages, size_t innerRuntimeIterations, size_t packagesize_tmp, double time_){
    package_vector.at(idx_numberofpackages) = packagesize_tmp;
    innerRuntimeIterations_vector.at(idx_numberofpackages)=innerRuntimeIterations;
    totaldatasent_vector.at(idx_numberofpackages)=packagesize_tmp*innerRuntimeIterations;
    
    int index = idx_outeriter*numberofpackages+idx_numberofpackages;
    time.at(index) = time_;
    
    summe.at(idx_numberofpackages) += time_;
    
    std::cout << time.at(index) << std::endl;
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