#from mateig import *
#import gridspec

def wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,Nconts=50,saveplot=None,rescale=None):
    if len(fld_list) != 4:
        print 'Too many! Only 4'
        return

    gmax_list = [x.g.max() for x in fld_list]

    fig = figure(figsize=(20,10));
    gs = gridspec.GridSpec(2,4)
    axes = [ subplot(g) for g in flatten(gs) ]
    for i,fld in enumerate(fld_list):
        ind1 = i*2
        ind2 = ind1+1
        fld.wkb(None,ax=axes[ind1],kmax=kmax_list[i],logr=False,softening=softening_list[i],Nconts=Nconts)

        if rescale != None:
            axes[ind1].set_ylim(rescale)

        if log_list[i]:
            axes[ind1].set_yscale('log')
            axes[ind1].yaxis.set_ticks_position('both')
            axes[ind1].tick_params(axis='y',reset=False,which='both',length=2,width=1)

        axes[ind1].set_title('$g_{max} = %.1f$' % gmax_list[i],fontsize=20)
        axes[ind2].set_xlabel('r',fontsize=20)

        if log_list[i]:
            axes[ind2].semilogx(fld.r,fld.Qbarr,'-k',label='Q-barrier')
            axes[ind2].semilogx(fld.r,fld.wp,'--k',label='$\\dot{\\varpi}$')
        else:
            axes[ind2].plot(fld.r,fld.Qbarr,'-k',label='Q-barrier')
            axes[ind2].plot(fld.r,fld.wp,'--k',label='$\\dot{\\varpi}$')
#        axes[ind2].legend(loc='best')
    for ax in axes[:4]:
        ax.set_xlabel('')

    for ax in axes[-4::2]:
        ax.set_xlabel('kr',fontsize=20)
        
    axes[2].set_ylabel('')
    axes[6].set_ylabel('')
    # for i in range(3):
    #     axes[i+1].set_ylabel('')
    #     axes[4+ i + 1].set_ylabel('')

    if saveplot != None:
        fig.savefig('plots/'+saveplot+'.png')
    return




def load_power_law_files():
    sigma0 = [1e-4, 1e-3, 3e-3, 1e-2]
    base='power_law_g_const_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [10,10,10,25]
    log_list = [True for x in fld]
    softening_list = [False for x in fld]
    rescale = (.1,10)
    return fld_list,log_list,kmax_list,softening_list,rescale

def load_inner_taper_files():
    sigma0 = [1e-4,1e-3,2e-3,1e-2]
    base='taper_f25_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [10,10,10,25]
    log_list = [True for x in fld]
    softening_list = [False for x in fld]
    rescale = (.1,10)
    return fld_list,log_list,kmax_list,softening_list,rescale

def load_gaussian_files():
    sigma0 = [1e-3,1e-2,3e-2,1e-1]
    base='gauss_f25_md0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [20,20,30,75]
    log_list = [False for x in fld]
    softening_list = [False for x in fld]
    rescale=None
    return fld_list,log_list,kmax_list,softening_list,rescale

def load_outer_taper_files():
    sigma0 = [ 1e-4 , 1e-3,  3e-3, 1e-2]
    base='outer_f25_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [5,10,10,25]
    log_list = [True for x in fld]
    softening_list = [False for x in fld]
    rescale = (.1,100)
    return fld_list,log_list,kmax_list,softening_list,rescale
