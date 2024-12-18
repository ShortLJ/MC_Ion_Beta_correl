
#include "Global.h"

#include "TreeImp.C"
#include "TreeDec.C"

#include "TTreeIndex.h"

void Sort()
{
	TFile *infile = new TFile("treeMC.root","read");
	TreeImp *pimp = new TreeImp((TTree*) infile->Get("treeImp"));
	pimp->fChain->LoadBaskets();
	TreeDec *pdec = new TreeDec((TTree*) infile->Get("treeDec"));
	pdec->fChain->LoadBaskets();

	Long64_t ient=0;


	TFile *file_out = new TFile("treeSort.root","recreate");
	TTree *treeImp = pimp->fChain->CloneTree(0);
	TTree *treeDec = pdec->fChain->CloneTree(0);

	Long64_t idx=0;
	Long64_t loop=0;
	TTreeIndex *treeidx;

	pimp->fChain->BuildIndex("Timp*1e9");
	treeidx = (TTreeIndex*)pimp->fChain->GetTreeIndex();
	for (idx=0; idx<treeidx->GetN(); idx++, loop++)
	{
		ient = treeidx->GetIndex()[idx];
		pimp->GetEntry( ient );
		if ((loop&0xfff)==0)
		{
			fprintf(stdout, "idx %lld ient %lld Timp %.01f\n", idx, ient, pimp->Timp);
		}
		if (pimp->Timp<ts_lowlimit || pimp->Timp>ts_upplimit) continue;
		treeImp->Fill();
	}
	fprintf(stdout, "idx %lld ient %lld Timp %.01f\n", idx, ient, pimp->Timp);
	fprintf(stdout, "loop %lld idx %lld ient %lld\n", loop, idx, ient);
	treeImp->Write();

	pdec->fChain->BuildIndex("Tdec*1e9");
	treeidx = (TTreeIndex*)pdec->fChain->GetTreeIndex();
	for (idx=0; idx<treeidx->GetN(); idx++, loop++)
	{
		ient = treeidx->GetIndex()[idx];
		pdec->GetEntry( ient );
		if ((loop&0xfff)==0)
		{
			fprintf(stdout, "idx %lld ient %lld Tdec %.01f\n", idx, ient, pdec->Tdec);
		}
		if (pdec->Tdec<ts_lowlimit || pdec->Tdec>ts_upplimit) continue;
		treeDec->Fill();
	}
	fprintf(stdout, "idx %lld ient %lld Tdec %.01f\n", idx, ient, pdec->Tdec);
	fprintf(stdout, "loop %lld idx %lld ient %lld\n", loop, idx, ient);
	treeDec->Write();





}
