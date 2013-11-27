#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getloadavg.c 8.1.1 (2.11BSD GTE) 1/15/95";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/vmparam.h>

/* getloadavg() -- Get system load averages.

Put `nelem' samples into `loadavg' array.
Return number of samples retrieved, or -1 on error.*/

int
getloadavg(loadavg, nelem)
double loadavg[];
register int nelem;
{
    struct loadavg loadinfo;
    register int i;
    int mib[2];
    size_t size;
    mib[0] = CTL_VM;
    mib[1] = VM_LOADAVG;
    size = sizeof(loadinfo);
    if (sysctl(mib, 2, &loadinfo, &size, NULL, 0) < 0)
        return (-1);

    nelem = MIN(nelem, sizeof(loadinfo.ldavg) / sizeof(short));
    for (i = 0; i < nelem; i++)
        loadavg[i] = (double) loadinfo.ldavg[i] / loadinfo.fscale;
    return (nelem);
}