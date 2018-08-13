nh: negatively charged hadrons, ph: positively charged hadrons

dpt: positively charged d-quark + negatively charged d-quark, 
upt: positively charged u-quark + negatively charged u-quark, 
up: positively charged u-quarks, un: negatively charged u-quarks, 
dp: positively charged d-quarks, dn: negatively charged d-quarks, 
g: gluons

bfn: bound and free proton or pPb collision system using EPPS16+CT14nlo and CT14nlo, 
bbn: bound and bound proton or PbPb collision system using EPPS16+CT14nlo, 
ffn: free and free proton or pp collision system using CT14nlo

A plot can have the switch HardQCDAll:hqa, HardQCDGluon:hqg, HardQCDQuark:hqq, along with the choice of collision system along with the parton involved in generating the hadrons per collision. 


If the x-axis has values spread throughout the range 1-500 (GeV/c), then the x-axis corresponds to a parton , i.e. for that particular parton involved in the collision, the ratio of positive and negative hadrons are obtained as a projection from partons:hadrons from 2D histograms per collision system 1M events. The 2D histograms start with the name h2\_
Eg:  hVSp_un_ph_hqg_bbn_BY_un_nh_hqg_bbn.png, implies, for the PbPb collision system and process HardQCDGluon, the ratio of positive/negative charged hadrons are plotted as a function of pT of the parton negative u-quark. 

(3-process switches)x(3:collision systems)x(ratios neg/pos, parton-1/parton-2, etc)

The images are availables in the directory `plots` which are processed based on the root files in the directory `files`
