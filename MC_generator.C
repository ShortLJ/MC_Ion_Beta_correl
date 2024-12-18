
#include "Global.h"


#include "TRandom.h"
#include "TH2I.h"
#include "THStack.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"


int MC_generator()
{
	vector<pair<double,double>> beam_up;
	beam_up.push_back(make_pair( -1800,15000));

///// gamma histogram mimic /// not implemented yet
/*
	double gamma_eff_peak[Nspecies][MaxDecChain]= // gamma efficiency * branching ratio
		{
			{0.1,0,0,0},
			{0,0,0,0}
		};
	double gamma_eff_bg[Nspecies][MaxDecChain]= // compton background level in ROI // integral // get these from geant4 
		{
			{0.01, 1,0,1},
			{1,1,1,1}
		};
*/

	double beam_rate[Nspecies] = BEAM_RATE;
	double beam_profile[2] = BEAM_PROFILE;
	double beta_pos_dev = BETA_POS_DEV;

	double Timp, Tdec[MaxDecChain], dTdecTimp[MaxDecChain];
	double Ximp,Yimp, Xdec[MaxDecChain], Ydec[MaxDecChain];
	Long64_t ievt=0;
	int ispecies, idx;

	TH1I *h1_Timp =new TH1I("h1_Timp","h1_Timp",4000,ts_lowlimit,ts_upplimit);
	TH1I *h1_Tdec =new TH1I("h1_Tdec","h1_Tdec",4000,ts_lowlimit,ts_upplimit);
	THStack *hs1_dTdecTimp[Nspecies];
	TH1I *h1_dTdecTimp[Nspecies][MaxDecChain];
	for (ispecies=0; ispecies<Nspecies; ispecies++)
	{
		hs1_dTdecTimp[ispecies] = new THStack(Form("hs1_dTdecTimp_%d",ispecies),Form("hs1_dTdecTimp_%d; Tdec-Timp [s]",ispecies));
		for (idx=0; idx<MaxDecChain; idx++)
		{
			h1_dTdecTimp[ispecies][idx] =new TH1I(
					Form("h1_dTdecTimp_%d_%d",ispecies,idx),
					Form("h1_dTdecTimp_%d_%d",ispecies,idx),
					4000,-200,200);
			h1_dTdecTimp[ispecies][idx]->SetLineColor(2+idx);
			hs1_dTdecTimp[ispecies]->Add(h1_dTdecTimp[ispecies][idx]);
		}
	}
	TH2D *h2_Ximp_Yimp = new TH2D("h2_Ximp_Yimp","Hit rate / 1mm^2; Ximp; Yimp",STOPPERX,-STOPPERX/2,STOPPERX/2, STOPPERY,-STOPPERY/2,STOPPERY/2);
	TH2I *h2_dXdecXimp_dYdecYimp = new TH2I("h2_dXdecXimp_dYdecYimp","h2_dXdecXimp_dYdecYimp; Xdec-Ximp; Ydec-Yimp",200,-50,50, 200,-50,50);

	TFile *file_out = new TFile("treeMC.root","recreate");

	TTree *treeMC = new TTree("treeMC","treeMC");
	treeMC->Branch("ievt",&ievt,"ievt/L");
	treeMC->Branch("ispecies",&ispecies,"ispecies/i");
	treeMC->Branch("Timp",&Timp, "Timp/D");
	treeMC->Branch(Form("Tdec[%d]",MaxDecChain),Tdec, Form("Tdec[%d]/D",MaxDecChain));
	treeMC->Branch(Form("dTdecTimp[%d]",MaxDecChain),dTdecTimp, Form("dTdecTimp[%d]/D",MaxDecChain));

	TTree *treeImp = new TTree("treeImp","treeImp");
	treeImp->Branch("ievt",&ievt,"ievt/L");
	treeImp->Branch("ispecies",&ispecies,"ispecies/i");
	treeImp->Branch("Timp",&Timp,"Timp/D");
	treeImp->Branch("Ximp",&Ximp,"Ximp/D");
	treeImp->Branch("Yimp",&Yimp,"Yimp/D");

	double Tdec_, Xdec_, Ydec_;
	TTree *treeDec = new TTree("treeDec","treeDec");
	treeDec->Branch("ievt",&ievt,"ievt/L");
	treeDec->Branch("idx",&idx,"idx/I");
	treeDec->Branch("Tdec",&Tdec_,"Tdec/D");
	treeDec->Branch("Xdec",&Xdec_,"Xdec/D");
	treeDec->Branch("Ydec",&Ydec_,"Ydec/D");


	double tau[Nspecies][MaxDecChain];
	for (ispecies=0; ispecies<Nspecies; ispecies++) for (idx=0; idx<MaxDecChain; idx++)	
		tau[ispecies][idx] = halflife[ispecies][idx]/log(2);
	double totalbeamrate=0;
	for (ispecies=0; ispecies<Nspecies; ispecies++) 
		totalbeamrate += beam_rate[ispecies];
	double frac_beam[Nspecies]={0};
	for (ispecies=0; ispecies<Nspecies; ispecies++) 
	{
		frac_beam[Nspecies-1] += beam_rate[ispecies]/totalbeamrate;
		frac_beam[ispecies] = frac_beam[Nspecies-1];
	}
	double uptime=0;
	for (size_t iup=0; iup<beam_up.size(); iup++)
	{
		uptime += beam_up[iup].second-beam_up[iup].first;
	}

	fprintf(stdout, "%f %f, %f \n",beam_rate[0],beam_rate[1], totalbeamrate);
	fprintf(stdout, "%f %f \n",frac_beam[0],frac_beam[1]);

	unsigned int nbu = beam_up.size();
	for (unsigned int ibu=0; ibu<nbu; ibu++)
	{
		Long64_t ievt0=ievt;
		Long64_t nevt = totalbeamrate * (beam_up.at(ibu).second - beam_up.at(ibu).first);
		for (ievt = ievt0; ievt<ievt0+nevt; ievt++)
		{
			if ((ievt&0xfff)==0)
			{
				fprintf(stdout, "\rievt %lld, ievt_local %lld of %lld",ievt,ievt-ievt0,nevt);
			}
			double rand_species = gRandom->Uniform(0,1);
			for (ispecies=0; ispecies<Nspecies; ispecies++)
			{
				if (rand_species<frac_beam[ispecies]) break;
			}
			if (ispecies==Nspecies){ievt--; continue;}

			Timp = gRandom->Uniform(beam_up.at(ibu).first,beam_up.at(ibu).second);
			Ximp = gRandom->Gaus(0,beam_profile[0]);
			Yimp = gRandom->Gaus(0,beam_profile[1]);
			if (Ximp>STOPPERX/2 || Ximp<-STOPPERX/2) continue;
			if (Yimp>STOPPERY/2 || Yimp<-STOPPERY/2) continue;

			treeImp->Fill();
			h1_Timp->Fill(Timp, 1);
			h2_Ximp_Yimp->Fill(Ximp,Yimp,1./uptime);


			Tdec_ = Timp;
			for (idx=0; idx<MaxDecChain; idx++) if (tau[ispecies][idx]>0)
			{
				Tdec_ += gRandom->Exp(tau[ispecies][idx]);
				Xdec_ = gRandom->Gaus(Ximp, beta_pos_dev); 
				Ydec_ = gRandom->Gaus(Yimp, beta_pos_dev); 
				treeDec->Fill();

				Tdec[idx]=Tdec_;
				dTdecTimp[idx] = Tdec[idx]-Timp;
				Xdec[idx] = Xdec_;
				Ydec[idx] = Ydec_;

				h1_Tdec->Fill(Tdec[idx], 1.);
				h1_dTdecTimp[ispecies][idx]->Fill(dTdecTimp[idx], 1.);
				h2_dXdecXimp_dYdecYimp->Fill(Xdec[idx]-Ximp, Ydec[idx]-Yimp, 1.);
			}
			treeMC->Fill();
		}
		fprintf(stdout, "\rievt %lld, ievt_local %lld of %lld",ievt,ievt0,nevt);
		fprintf(stdout, "\n");
	}

	TVirtualPad *pad;

	TCanvas *c1 = new TCanvas("MC","MC",1200,1000);
	c1->Divide(1,4);

	pad = c1->cd(1);
	pad->SetGridx();
	h1_Timp->Draw();

	c1->cd(2)->Divide(Nspecies,1);
	for (ispecies=0; ispecies<Nspecies; ispecies++)
	{
		pad = c1->cd(2)->cd(1+ispecies);
		pad->SetGridx();
		hs1_dTdecTimp[ispecies]->Draw("nostack");
	}

	pad = c1->cd(3);
	pad->SetGridx();
	h1_Tdec->Draw();

	c1->cd(4)->Divide(2,1);
	pad = c1->cd(4)->cd(1);
	h2_Ximp_Yimp->Draw("colz");
	pad = c1->cd(4)->cd(2);
	h2_dXdecXimp_dYdecYimp->Draw("colz");

	treeMC->Write();
	treeImp->Write();
	treeDec->Write();
	c1->Write();

	
	return 0;
}
