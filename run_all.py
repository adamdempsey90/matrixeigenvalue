import os
import eigen
from copy import deepcopy


def run_one(params, add_defs, rm_defs,outdir):

    add_remove_option(add_defs,rm_defs,recompile=True)
    run_code(params)


prof = 'taper'
fame = 'results'
alpha = [0,.0001,.001,.01]
sigma = [.001,.003,.01,.03]
eps = {'rsconst': .017, 'rslin': .03}
num = {'low': 400, 'hi': 800}
params = eigen.load_params()


# Remove openmp if doing jobs in parallel

def_list =[]
add_list = []
rm_list = []

for eos in ['baro','iso']:

    if eos == 'baro':
        add_defs = ['BAROTROPIC']
        rm_defs = ['ISOTHERMAL']
    else:
        add_defs = ['ISOTHERMAL']
        rm_defs = ['BAROTROPIC']

    for eint in ['ext','noext']:

        if eint == 'ext':
            add_defs.append('EXTENDINTEG')
        else:
            rm_defs.append('EXTENDINTEG')

        for soft in ['rsconst','rslin']:

            params['rs'] = eps[soft]
            if soft == 'rsconst':
                add_defs.append('CONSTSOFT')
            else:
                rm_defs.append('CONSTSOFT')

            for res in ['low','hi']:
                params['nr'] = num[res]

                for sig in sigma:
                    sigstr = 'sig%.e' % sig
                    params['mdisk'] = sig

                    for a in alpha:
                        astr = 'a%.e' % a
                        params['alpha_s'] = a

                        params_list.append(deepcopy(params))
                        add_list.append(add_defs)
                        rm_list.append(rm_defs)


                        dir_list = [prof,eos,eint,soft,res,sigstr,astr]

                        dirname = '/'.join(dir_list) + '/'
                        fname = dirname + fname
