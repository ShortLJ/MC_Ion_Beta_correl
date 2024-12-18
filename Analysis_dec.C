
#include "Global.h"

#include "TreeImp.C"
#include "TreeDec.C"


void Analysis_dec(int nset=1, int iset=0)
{
	TreeDec *pdec = new TreeDec();
	Long64_t nent_dec = pdec->fChain->GetEntries();
	fprintf(stdout,"%lld entries\n",nent_dec);
	pdec->fChain->LoadBaskets();

	TH1D *h1_Tdec =new TH1D("h1_Tdec","h1_Tdec; Tdec [s]; rate, 100s avg",(ts_upplimit-ts_lowlimit)/100,ts_lowlimit,ts_upplimit);

	Long64_t loop=0;
	Long64_t ient_dec=0;
	UInt_t ndec = 0;

	for (ient_dec=0; ient_dec<nent_dec; ient_dec++,loop++)
	{
		pdec->GetEntry(ient_dec);
		double Tdec = pdec->Tdec;
		if (Tdec<ts_lowlimit || Tdec>ts_upplimit) continue;

		//// fill histograms /// 
		h1_Tdec->Fill(pdec->Tdec,0.01);
	}

	//for (ient_imp=0; ient_imp<nent_imp; ient_imp++, loop++)
	//{
	//	pimp->GetEntry(ient_imp);
	//	h1_Timp->Fill(pimp->Timp);
	//}
	//for (ient_dec=0; ient_dec<nent_dec; ient_dec++, loop++)
	//{
	//	pdec->GetEntry(ient_dec);
	//	h1_Tdec->Fill(pdec->Tdec);
	//}

	TVirtualPad *pad;

	TCanvas *c1 = new TCanvas("Analysis","Analysis",1200,1000);
	h1_Tdec->Draw();

	TFile *fileout = new TFile(Form("hists_dec.root"),"recreate");
	h1_Tdec->Write();



}
