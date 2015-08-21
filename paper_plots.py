#from mateig import *
#import gridspec

def wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,Nconts=50,saveplot=None,rescale=None):
    if len(fld_list) != 4:
        print 'Too many! Only 4'
        return

    if saveplot != None:
        rasterized = True
    else:
        rasterized = False

    gmax_list = [x.g.max() for x in fld_list]

    fig = figure(figsize=(20,10));
    gs = gridspec.GridSpec(2,4)
    axes = [ subplot(g,rasterized=rasterized) for g in flatten(gs) ]
    for i,fld in enumerate(fld_list):
        ind1 = i*2
        ind2 = ind1+1
        fld.wkb(ax=axes[ind1],kmax=kmax_list[i],logr=False,softening=softening_list[i],Nconts=Nconts)

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
        print '\tGenerating PNG...'
        fig.savefig('plots/PNG/'+saveplot+'.png',format='png')
        print '\tGenerating EPS...'
        fig.savefig('plots/EPS/'+saveplot+'.eps',format='eps')
        print '\tGenerating PDF...'
        fig.savefig('plots/PDF/'+saveplot+'.pdf',format='pdf')
    return


def generate_wkb_plots():

    print 'Power Law...'
    fld_list,log_list,kmax_list,softening_list,rescale,_ = load_power_law_files()
    softening_list2 = [True for s in softening_list]
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,saveplot='power_law_wkb_plots',rescale=rescale)
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list2,saveplot='power_law_soft_wkb_plots',rescale=rescale)

    print 'Inner Taper...'
    fld_list,log_list,kmax_list,softening_list,rescale,_ = load_inner_taper_files()
    softening_list2 = [True for s in softening_list]
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,saveplot='taper_wkb_plots',rescale=rescale)
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list2,saveplot='taper_soft_wkb_plots',rescale=rescale)

    print 'Outer Taper...'
    fld_list,log_list,kmax_list,softening_list,rescale,_ = load_outer_taper_files()
    softening_list2 = [True for s in softening_list]
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,saveplot='outer_taper_wkb_plots',rescale=rescale)
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list2,saveplot='outer_taper_soft_wkb_plots',rescale=rescale)

    print 'Gaussian Ring...'
    fld_list,log_list,kmax_list,softening_list,rescale,_ = load_gaussian_files()
    softening_list2 = [True for s in softening_list]
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list,saveplot='gaussian_wkb_plots',rescale=rescale)
    wkb_overview_plots(fld_list,log_list,kmax_list,softening_list2,saveplot='gaussian_soft_wkb_plots',rescale=rescale)

    print 'Done'

    return

def load_power_law_files(softening=False):
    sigma0 = [1e-4, 1e-3, 3e-3, 1e-2]
    base='power_law_g_const_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [10,10,10,30]
    log_list = [True for x in fld_list]
    softening_list = [softening for x in fld_list]
    rescale = (.1,10)
    mode_list = [[-3, -5,-10,-20],[-1,-2,-3,-7],[-1,-4,-8,-11],[-3,-10,-16,-25]]
    return fld_list,log_list,kmax_list,softening_list,rescale,mode_list

def load_inner_taper_files(softening=False):
    sigma0 = [1e-4,1e-3,2e-3,1e-2]
    base='taper_f25_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [10,10,10,25]
    log_list = [True for x in fld_list]
    softening_list = [softening for x in fld_list]
    rescale = (.1,10)
    mode_list = [[-3,-10,-20,-30], [-1,-2,-5,-15], [-2,-3,-6,-15],[-1,-6,-14,-20]]
    return fld_list,log_list,kmax_list,softening_list,rescale,mode_list

def load_gaussian_files(softening=False):
    sigma0 = [1e-3,1e-2,3e-2,1e-1]
    base='gauss_f25_md0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [30,30,30,70]
    log_list = [False for x in fld_list]
    softening_list = [softening for x in fld_list]
    rescale=None
    mode_list = [[-3,-5,-10,-15], [-1,-4,-7,-10],[-1,-2,-5,-10],[-1,-3,-9,-12]]
    return fld_list,log_list,kmax_list,softening_list,rescale,mode_list

def load_outer_taper_files(softening=False):
    sigma0 = [ 1e-4 , 1e-3,  3e-3, 1e-2]
    base='outer_f25_s0_'
    fnames = [base + '%.0e.hdf5'%s for s in sigma0]
    fld_list = [Field(x) for x in fnames]
    kmax_list = [5,10,10,25]
    log_list = [True for x in fld_list]
    softening_list = [softening for x in fld_list]
    rescale = (.1,100)
    mode_list = [[],[],[],[]]
    return fld_list,log_list,kmax_list,softening_list,rescale,mode_list



def taper_mode_plots(fld_list,log_list,kmax_list,softening_list,mode_list,saveplot=None,rescale=None):

    if saveplot != None:
        rasterized = True
    else:
        rasterized = False

    fig = figure(figsize=(30,15))

    gs = gridspec.GridSpec(2,4)
    gmax_list = [fld.g.max() for fld in fld_list]
    axes = [ subplot(g,rasterized=rasterized) for g in flatten(gs) ]
    for i in range(len(fld_list)):
        axw = axes[2*i]
        axe = axes[2*i+1]
        gmax = gmax_list[i]
        fld = fld_list[i]
        logr = log_list[i]
        kmax = kmax_list[i]
        softening = softening_list[i]
        modes = mode_list[i]

#        fig,(axw,axe) = subplots(1,2,sharey='row',figsize=(20,10))

        soms = array([fld.evals[x] for x in modes])
        cvals = ['b','g','m','c']

        kk,rr,omp=fld.wkb(ax=axw,kmax=kmax,logr=logr,softening=softening,use_ev=True,rescale=rescale,returnomp=True)

        axw.contour(kk,rr,omp,levels=soms.real,colors=cvals,linewidths=4,linestyles='-');
        axw.set_title('$g_{max} = %.1f$' % gmax,fontsize=20)
        for ev,c in zip(soms,cvals):
            emode = fld.edict[ev]
            if abs(ev.imag) < 1e-9:
                label = '$\\Omega_p = %.2e$' % ev.real
            else:
                label = '$\\Omega_p = %.2e + %.2ei$'%(ev.real,ev.imag)
            axe.plot(emode.real,fld.r,'-'+c,linewidth=4,label=label)
            axe.plot(emode.imag,fld.r,'--'+c,linewidth=4)


        axe.set_xlabel('$e(r)$',fontsize=20)
        if rescale:
            axe.set_ylim(rescale)
        if logr:
            axe.set_yscale('log')
        axe.legend(loc='upper right')

    subplots_adjust(wspace=0)

    for i,ax in enumerate(axes):
        if i not in [0,4]:
            ax.set_ylabel('')
            ax.tick_params(axis='y',labelleft='off')


    for ax in axes[:4]:
        ax.set_xlabel('')

    if saveplot != None:
        print '\tGenerating PNG...'
        fig.savefig('plots/PNG/'+saveplot+'.png',format='png')
        print '\tGenerating EPS...'
        fig.savefig('plots/EPS/'+saveplot+'.eps',format='eps')
        print '\tGenerating PDF...'
        fig.savefig('plots/PDF/'+saveplot+'.pdf',format='pdf')

    return

def generate_mode_plots():
    print 'Power Law...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_power_law_files()
    softening_list2 = [True for s in softening_list]
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list,mode_list,saveplot='power_law_modes_plot',rescale=rescale)
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list2,mode_list,saveplot='power_law_soft_modes_plot',rescale=rescale)

    print 'Inner Taper...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_inner_taper_files()
    softening_list2 = [True for s in softening_list]
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list,mode_list,saveplot='taper_modes_plot',rescale=rescale)
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list2,mode_list,saveplot='taper_soft_modes_plot',rescale=rescale)

    # print 'Outer Taper...'
    # fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_outer_taper_files()
    # softening_list2 = [True for s in softening_list]
    # taper_mode_plots(fld_list,log_list,kmax_list,softening_list,mode_list,saveplot='outer_taper_modes_plot',rescale=rescale)
    # taper_mode_plots(fld_list,log_list,kmax_list,softening_list2,mode_list,saveplot='outer_taper_soft_modes_plot',rescale=rescale)

    print 'Gaussian Ring...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list= load_gaussian_files()
    softening_list2 = [True for s in softening_list]
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list,mode_list,saveplot='gaussian_modes_plot',rescale=rescale)
    taper_mode_plots(fld_list,log_list,kmax_list,softening_list2,mode_list,saveplot='gaussian_soft_modes_plot',rescale=rescale)

    print 'Done'

    return

def mode_summary_plot(fld_list,log_list,saveplot=None,rescale=None,plotilr=False):
    rasterized = (saveplot != None)
    gmax_list = [fld.g.max() for fld in fld_list]

    for num,(fld,logr,gmax) in enumerate(zip(fld_list,log_list,gmax_list)):
        fname_suff = '_%d'%(num+1)

        # fig = figure(figsize=(20,10))
        # gs = gridspec.GridSpec(3,3)
        # axes = [ subplot(g,rasterized=rasterized) for g in flatten(gs) ]
        fig,axes = subplots(3,3,sharex='col',figsize=(20,10))


        npr = len(fld.evals[(fld.evals>0)&(abs(fld.evals.real)<1)])

        if (npr > 5):       # Too many prograde modes
            prograde = fld.evals[(fld.evals>0)&(abs(fld.evals.real)<1)]
            retrograde = fld.evals[(fld.evals<0)&(abs(fld.evals.real)<1)]
            omp = hstack((prograde[-5:][::-1],retrograde[-4:][::-1]))
        else:
            omp = fld.evals[(fld.evals != 0)&(abs(fld.evals.real)<1)][-9:][::-1]

        emodes = [fld.edict[ev] for ev in omp]

        if plotilr:
            ilr = []
            for ev in omp:
                ilr.append(fld.r[sign(fld.wp-ev.real)[1:] - sign(fld.wp-ev.real)[:-1] != 0])


        for i,(ax,ev,em) in enumerate(zip(flatten(axes),omp,emodes)):

            ax.plot(fld.r,em.real,'-k',fld.r,em.imag,'--k')
            if plotilr:
                if len(ilr[i]) > 0:
                    for lr in ilr[i]:
                        ax.axvline(lr,color='k',linestyle='-',linewidth=2)

            if abs(ev.imag) < 1e-9:
                tstr = '$\\Omega_p = %.2e$' % ev.real
            else:
                tstr = '$\\Omega_p =  %.2e + %.2ei$' % (ev.real,ev.imag)

            if i==0:
                gstr = '$g_{max} = %.1f$' % gmax
                tstr = gstr + '\t' +tstr

            ax.set_title(tstr,fontsize=20)

            if rescale != None:
                ax.set_xlim(rescale)
            if logr:
                ax.set_xscale('log')

        for ax in axes[2]:
            ax.set_xlabel('$r$',fontsize=20)
        for ax in axes:
            ax[0].set_ylabel('$e$',fontsize=20)

        if saveplot != None:
            print '\tGenerating PNG...'
            fig.savefig('plots/PNG/'+saveplot+fname_suff + '.png',format='png')
            print '\tGenerating EPS...'
            fig.savefig('plots/EPS/'+saveplot+fname_suff + '.eps',format='eps')
            print '\tGenerating PDF...'
            fig.savefig('plots/PDF/'+saveplot+fname_suff + '.pdf',format='pdf')
#    subplots_adjust(wspace=0)


    return

def generate_single_modes_plots(plotilr=False):
    fname_base = '_indv_modes_plot'
    if plotilr:
        fname_base = '_ilr' + fname_base

    power_fname = 'power_law' + fname_base
    taper_fname = 'taper'+ fname_base
    outer_fname = 'outer_taper'+ fname_base
    gauss_fname = 'gaussian'+ fname_base


    print 'Power Law...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_power_law_files()
    mode_summary_plot(fld_list,log_list,saveplot=power_fname,rescale=rescale,plotilr=plotilr)


    print 'Inner Taper...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_inner_taper_files()
    mode_summary_plot(fld_list,log_list,saveplot=taper_fname,rescale=rescale,plotilr=plotilr)

    print 'Outer Taper...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list = load_outer_taper_files()
    mode_summary_plot(fld_list,log_list,saveplot=outer_fname,rescale=rescale,plotilr=plotilr)

    print 'Gaussian Ring...'
    fld_list,log_list,kmax_list,softening_list,rescale,mode_list= load_gaussian_files()
    mode_summary_plot(fld_list,log_list,saveplot=gauss_fname,rescale=rescale,plotilr=plotilr)

    print 'Done'

    return
