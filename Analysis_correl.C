
#include "Global.h"

#include "TreeImp.C"
#include "TreeDec.C"


void Analysis_correl(int nset=1, int iset=0)
{
	TFile *infile = new TFile("treeSort.root","read");
	TreeImp *pimp = new TreeImp((TTree*) infile->Get("treeImp"));
	Long64_t nent_imp = pimp->fChain->GetEntries();
	fprintf(stdout,"%lld entries\n",nent_imp);
	pimp->fChain->LoadBaskets();
	TreeDec *pdec = new TreeDec((TTree*) infile->Get("treeDec"));
	Long64_t nent_dec = pdec->fChain->GetEntries();
	fprintf(stdout,"%lld entries\n",nent_dec);
	pdec->fChain->LoadBaskets();

	double timewindowL=64 * TIME_SCALE;
	double timewindowS= 2 * TIME_SCALE;
	double length = CORREL_LENGTH;

	TH1I *h1_dTdecTimp = new TH1I("h1_dTdecTimp","h1_dTdecTimp", 640,-timewindowL,timewindowL);
	TH1D *h1_Timp_total =new TH1D("h1_Timp_total","h1_Timp_total; Timp [s]; rate, 100s avg",(ts_upplimit-ts_lowlimit)/100,ts_lowlimit,ts_upplimit);
	TH1D *h1_Timp_target =new TH1D("h1_Timp_target","h1_Timp_target; Timp [s]; rate, 100s avg",ts_upplimit/100,0,ts_upplimit);
	TH1D *h1_Tdec =new TH1D("h1_Tdec","h1_Tdec; Tdec [s]; count/100s",(ts_upplimit-ts_lowlimit)/100,ts_lowlimit,ts_upplimit);
	TH2D *h2_dXdecXimp_dYdecYimp = new TH2D("h2_dXdecXimp_dYdecYimp","h2_dXdecXimp_dYdecYimp; Xdec-Ximp; Ydec-Yimp",200,-50,50, 200,-50,50);
	h2_dXdecXimp_dYdecYimp->Sumw2();

	h1_dTdecTimp->SetTitle(Form("h1_dTdecTimp; T(dec)-T(imp) [s]; count/%.0f us", 1000*h1_dTdecTimp->GetXaxis()->GetBinWidth(1)));

	Long64_t loop=0;
	Long64_t ient_imp=0, ient_dec=0;
	Long64_t first_ient_dec=0;
	UInt_t ndec = 0;

	Long64_t ient_start = (iset)*nent_imp/nset; 
	Long64_t ient_stop = (iset+1)*nent_imp/nset;
	if (iset<0)
	{
		ient_start=0;
		ient_stop=10;
	}

	for (ient_imp=ient_start; ient_imp<ient_stop; ient_imp++, loop++)
	{
		pimp->GetEntry(ient_imp);
		double Timp = pimp->Timp;
		if (Timp<ts_lowlimit+timewindowL) continue;
		if (Timp>ts_upplimit-timewindowL) continue;

		h1_Timp_total->Fill(pimp->Timp, 0.01);
		if (pimp->ispecies!=0) continue;
		h1_Timp_target->Fill(pimp->Timp,0.01);

		//if (Timp<400+timewindowL+120) continue;
		//if (Timp<400+4*100) continue;
		ndec=0;

		for (ient_dec=first_ient_dec; ient_dec<nent_dec; ient_dec++,loop++)
		//for (ient_dec=0; ient_dec<nent_imp; ient_dec++,loop++)
		{
			pdec->GetEntry(ient_dec);
			double Tdec = pdec->Tdec;
			double dTdecTimp = Tdec-Timp;
			if (dTdecTimp<-timewindowL)
			{
				first_ient_dec = ient_dec;
				continue;
			}
			if (dTdecTimp>= timewindowL)
			{
				break;
			}
			ndec++;
			if ((loop&0x7fffff)==0) 
			{
				fprintf(stdout,"ient_imp %lld ient_dec %lld ndec %u Timp %.1f Tdec %.1f \n", ient_imp,ient_dec,ndec, Timp,Tdec);
			}

			bool flag_short_time_window=0;
			if (dTdecTimp>=-timewindowS && dTdecTimp<timewindowS) flag_short_time_window=1;

			double dXdecXimp =  pdec->Xdec - pimp->Ximp;
			double dYdecYimp =  pdec->Ydec - pimp->Yimp;
			bool flag_pos_correl=0;
			if (pow(dXdecXimp/1,2)+pow(dYdecYimp/1,2)<pow(length,2)) flag_pos_correl = 1;


//// fill histograms /// 
			h1_Tdec->Fill(pdec->Tdec);
			if (flag_short_time_window)
			{
				if (dTdecTimp>=0)
				{
					h2_dXdecXimp_dYdecYimp->Fill(dXdecXimp,dYdecYimp,1);
				}
				if (dTdecTimp <0) 
				{
					h2_dXdecXimp_dYdecYimp->Fill(dXdecXimp,dYdecYimp,-1);
				}
			}

			if (flag_pos_correl)
			{
				h1_dTdecTimp->Fill(dTdecTimp);
			}

		}
	}
	fprintf(stdout,"\rient_imp %lld of nent_imp %lld ient_dec %lld of nent %lld", ient_imp,nent_imp, ient_dec,nent_dec);
	fprintf(stdout,"\n");

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
	c1->Divide(1,3);

	pad = c1->cd(1);
	pad->SetGridx();
	pad->SetLogy();
	h1_Timp_total->Draw();
	h1_Timp_target->Draw("same");

	pad = c1->cd(2);
	pad->SetGridx();
	h1_Tdec->Draw();

	pad = c1->cd(3);
	pad->SetGridx();
	h1_dTdecTimp->Draw();


	TFile *fileout = new TFile(Form("tmp_hists_%02d.root",iset),"recreate");
	h1_Timp_total->Write();
	h1_Timp_target->Write();
	h1_dTdecTimp->Write();
	h1_Tdec->Write();
	h2_dXdecXimp_dYdecYimp->Write();



}
