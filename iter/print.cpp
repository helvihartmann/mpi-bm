#include "print.h"
/* pfad
27.11.2013*/

void Printoutput::printtimestemp(){
        time_t Zeitstempel;
        tm *nun;
        Zeitstempel = time(0);
        nun = localtime(&Zeitstempel);
        std::cout << "#" << nun->tm_mday << '.' << nun->tm_mon+1 << '.'<< nun->tm_year+1900 << " - " << nun->tm_hour << ':' << nun->tm_min << std::endl;
}
    
void Printoutput::printheader(){
        printf("# repeats  packagesize time sendbandwidth varianz receivebandwidth varianz load last: 1min 5min 15min \n");
}
    
void Printoutput::printerrormessage(size_t *everythingcorrect_check,size_t packagesize_temp){
        printf("%ld errors were spotted sending %ld Byte\n", *everythingcorrect_check,packagesize_temp);
}
    
void Printoutput::printbandwidth(size_t innerRuntimeIterations, size_t packagesize_temp, double send_mean, long double send_rate, double send_var, long double receive_rate, double recv_var, double loadavg[]){
        printf("%ld %ld %f %Lf %f %Lf %f - %f %f %f \n",innerRuntimeIterations, packagesize_temp*sizeof(int),send_mean, send_rate, send_var,receive_rate, recv_var, loadavg[0], loadavg[1], loadavg[2]);
}
    
