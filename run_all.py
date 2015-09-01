
import os
from subprocess import call
import eigen
from copy import deepcopy
import multiprocessing


def mv_files((params, add_defs,rm_defs,outdir)):
    eigen.add_remove_option(add_defs,rm_defs,recompile=True)
    eigen.dump_params(params)

    callstr = ['cp','params.opt','params.in','eigen',outdir]
    print '\t\t',' '.join(callstr)
    call(callstr)


    return

def run_one(outdir):
    os.chdir(outdir)
    callstr = [outdir + 'eigen', '-i', outdir + 'params.in']
    call(callstr)

def make_dir(dname):
    try:
        os.mkdir(dname)
    except:
        pass
    return

def setup_files(dir_base,prof,alpha= [0,.0001,.001,.01],sigma= [.001,.003,.01,.03],parallel=False):

# dir_base = '/Users/zeus/disk_res/'
# prof = 'taper'

    fname = 'results'

    eps = {'rsconst': .017, 'rslin': .03}
    num = {'low': 400, 'hi': 800}


    add_dict = {'baro':'BAROTROPIC','iso':'ISOTHERMAL','ext':'EXTENDINTEG','rsconst':'CONSTSOFT'}
    rm_dict = {'baro':'ISOTHERMAL','iso':'BAROTROPIC','noext':'EXTENDINTEG','rslin':'CONSTSOFT'}



    params = eigen.load_params()


    # create directory string
    # create parameter file
    # create option file
    #

    # Remove openmp if doing jobs in parallel
    if parallel:
        eigen.remove_option('OPENMP')

    def_list =[]
    add_list = []
    rm_list = []
    dirname_list =[]
    params_list = []

    make_dir(dir_base)
    dir_base += prof
    make_dir(dir_base)

    for eos in ['baro','iso']:
        dir_list = [dir_base,eos]
        make_dir('/'.join(dir_list))

        for eint in ['ext','noext']:
            dir_list = [dir_base,eos,eint]
            make_dir('/'.join(dir_list))

            for soft in ['rsconst','rslin']:
                dir_list = [dir_base,eos,eint,soft]
                make_dir('/'.join(dir_list))

                params['rs'] = eps[soft]

                for res in ['low','hi']:
                    dir_list = [dir_base,eos,eint,soft,res]
                    make_dir('/'.join(dir_list))

                    params['nr'] = num[res]

                    for sig in sigma:
                        sigstr = 'sig%.e' % sig
                        dir_list = [dir_base,eos,eint,soft,res,sigstr]
                        make_dir('/'.join(dir_list))


                        params['mdisk'] = sig

                        for a in alpha:
                            astr = 'a%.e' % a
                            dir_list = [dir_base,eos,eint,soft,res,sigstr,astr]
                            make_dir('/'.join(dir_list))

                            params['alpha_s'] = a
                            params['outputname'] = 'results'
                            params_list.append(deepcopy(params))
                            add_defs = [add_dict[key] for key in dir_list if key in add_dict.keys()]
                            rm_defs = [rm_dict[key] for key in dir_list if key in rm_dict.keys()]
                            add_list.append(add_defs)
                            rm_list.append(rm_defs)
                            fname = '/'.join(dir_list) + '/'
                            dirname_list.append(fname)


    return params_list,add_list,rm_list,dirname_list



if __name__ == "__main__":
    parallel = True
    params_list,add_list,rm_list,dirname_list = setup_files('/Users/zeus/disk_res/','taper',parallel=parallel)
    for x in zip(params_list,add_list,rm_list,dirname_list)[:4]:
        mv_files(x)
    if parallel:
        np = 2
        print 'Using %d threads' % np
        pool = multiprocessing.Pool(np)
        pool.map(run_one,dirname_list[:4])
    else:
        for x in dirname_list:
            run_one(x)
