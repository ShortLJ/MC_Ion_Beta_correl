

void fit_h1_dTdecTimp(TString filename="hists.root", int rebin=2)
{
	TFile *file_dec = new TFile("hists_dec.root","read");
	TH1 *h1_Tdec = (TH1*) file_dec->Get("h1_Tdec");
	TFile *file = new TFile(filename,"read");
	TH1D *h1_dTdecTimp = (TH1D*) file->Get("h1_dTdecTimp");

	TVirtualPad *pad;
	TCanvas *c2 = new TCanvas("c2","c2");


	gStyle->SetOptStat(0);
	TString str_sp="MC";
	TCanvas *c1 = new TCanvas("c1","c1",600,800);
	c1->Divide(1,2);

	pad = c1->cd(1);
	//pad->SetLogy();
	THStack *hs_Timp = new THStack("hs_Timp","hs_rate; T [s]; rate, 100s avg [1/s]");
	TH1 *h1_Timp_total = (TH1*)file->Get("h1_Timp_total");
	TH1 *h1_Timp_target = (TH1*)file->Get("h1_Timp_target"); 
	h1_Timp_total->SetLineColor(kBlue);
	h1_Timp_target->SetLineColor(kRed);
	h1_Tdec->SetLineColor(kGreen);
	hs_Timp->Add(h1_Timp_total);
	hs_Timp->Add(h1_Timp_target);
	hs_Timp->Add(h1_Tdec);
	hs_Timp->Draw("nostack hist");
	TLegend *lgd = new TLegend(0.8,0.8,0.9,0.9);
	lgd->AddEntry(h1_Timp_total,"total");
	lgd->AddEntry(h1_Timp_target,"target");
	lgd->AddEntry(h1_Tdec,"decay");
	lgd->Draw("same");

	c1->cd(2);
	h1_dTdecTimp->Rebin(rebin);
	h1_dTdecTimp->Draw("e");
	h1_dTdecTimp->GetYaxis()->SetRangeUser(0,1.1*h1_dTdecTimp->GetMaximum());
	h1_dTdecTimp->SetTitle(Form("T(dec)-T(imp) {%s}; Tdiff [s]; count / %.3f s", str_sp.Data(), h1_dTdecTimp->GetXaxis()->GetBinWidth(1)));

	//double bg = h1_dTdecTimp->GetBinContent(2);
	double bg = h1_dTdecTimp->GetBinContent(1+h1_dTdecTimp->GetXaxis()->GetNbins()/2-1);
	double peak = h1_dTdecTimp->GetBinContent(1+h1_dTdecTimp->GetXaxis()->GetNbins()/2+1) - bg;
	fprintf(stdout, "candi bg %f peak %f\n",bg,peak);

	/*
	//TF1 *f1 = new TF1("f111","[0]*log(2)/([1]-[2])*(0.5**(x/1e9/[1])+0.5**(x/1e9/[2]))*((x>0)?1:0) + [3]*(1+[4]*(x/1e9)+[5]*(x/1e9)**2)");
	//TF1 *f1 = new TF1("f111","[0]*log(2)/[1]/([2]/[1]-1)*(([2]/[1]-2)*0.5**(x/1e9/[1])+0.5**(x/1e9/[2]))*((x>0)?1:0) + [3]*(1+[4]*(x/1e9)+[5]*(x/1e9)**2)");
	TF1 *f1 = new TF1("f111","[0]/([2]/[1]-1)*(([2]/[1]-2)*0.5**(x/1e9/[1])+0.5**(x/1e9/[2]))*((x>0)?1:0) + [3]*(1+[4]*(x/1e9)+[5]*(x/1e9)**2)");
	//f1->SetParameters(peak,1,5,bg,0,0);  // 92Pd->92Rh
	//f1->SetParameters(peak,27,13000,bg,0,0);  // 94Rh->94Ru
	//f1->SetParameters(peak,1,12,bg,0,0);  // 93Pd->93Rh
	//f1->SetParameters(peak,5,3.65*60,bg,0,0); // 92Rh->92Ru
	//f1->SetParameters(peak,10,60,bg,0,0); // 94Pd->94Rh
	//f1->SetParLimits(1,0.1,15); f1->SetParLimits(2,5,12*2);
	//f1->SetParLimits(1,0.1,15); f1->SetParLimits(2,60,3.65*60*2);
	//f1->SetParLimits(1,0.1,10); f1->SetParLimits(2,3,50);
	f1->FixParameter(4,0);
	f1->FixParameter(5,0);
	f1->Print();
	h1_dTdecTimp->Fit(f1,"M+");
	*/

	TF1 *f1_lbate1 = new TF1("lbate1","[0]*(0.5**(x/[1])/[1])");
	TF1 *f1_lbate2 = new TF1("lbate2","[0]*(0.5**(x/[1])/[1]/(1-[2]/[1]) + 0.5**(x/[2])/[2]/(1-[1]/[2]))");
	TF1 *f1_lbate3 = new TF1("lbate3","[0]*(0.5**(x/[1])/[1]/(1-[2]/[1])/(1-[3]/[1]) + 0.5**(x/[2])/[2]/(1-[1]/[2])/(1-[3]/[2]) + 0.5**(x/[3])/[3]/(1-[1]/[3])/(1-[2]/[3]))");
	TF1 *f1_fbg = new TF1("fbg","[4]");

	TF1 *f1 = new TF1("f1","(lbate1(x)+lbate2(x)+lbate3(x))*((x>0)?1:0)+[4]"); 
	f1->Print();
	f1->SetParLimits(0,0.05*peak,100*peak);
	f1->SetParameters(peak,0.11,12,60,bg); f1->SetParLimits(1,0.01,4); f1->SetParLimits(2,6,60); f1->SetParLimits(3,40,120); // 93Pd->
	//f1->SetParameters(peak,5,3.65*60,4.3*60,bg); f1->SetParLimits(1,0.1,15); f1->SetParLimits(2,60,3.65*60*2); f1->SetParLimits(3,180,4.65*60*2); // 92Rh->
	//f1->FixParameter(2,12); 
	//f1->FixParameter(3,60);
	h1_dTdecTimp->Fit(f1,"I M+0");

	TF1 *f1_el1 = new TF1("el1","lbate1(x)*((x>0)?1:0)+[4]"); f1_el1->SetParameters(f1->GetParameters()); f1_el1->SetRange(h1_dTdecTimp->GetXaxis()->GetBinLowEdge(1), h1_dTdecTimp->GetXaxis()->GetBinUpEdge(1+h1_dTdecTimp->GetXaxis()->GetNbins()));
	TF1 *f1_el2 = new TF1("el2","lbate2(x)*((x>0)?1:0)+[4]"); f1_el2->SetParameters(f1->GetParameters()); f1_el2->SetRange(h1_dTdecTimp->GetXaxis()->GetBinLowEdge(1), h1_dTdecTimp->GetXaxis()->GetBinUpEdge(1+h1_dTdecTimp->GetXaxis()->GetNbins()));
	TF1 *f1_el3 = new TF1("el3","lbate3(x)*((x>0)?1:0)+[4]"); f1_el3->SetParameters(f1->GetParameters()); f1_el3->SetRange(h1_dTdecTimp->GetXaxis()->GetBinLowEdge(1), h1_dTdecTimp->GetXaxis()->GetBinUpEdge(1+h1_dTdecTimp->GetXaxis()->GetNbins()));
	TF1 *f1_full = (TF1*) f1->Clone(); f1_full->SetRange(h1_dTdecTimp->GetXaxis()->GetBinLowEdge(1), h1_dTdecTimp->GetXaxis()->GetBinUpEdge(1+h1_dTdecTimp->GetXaxis()->GetNbins())); 
	f1_el1->SetLineColor(kBlue);
	f1_el2->SetLineColor(kGreen);
	f1_el3->SetLineColor(kYellow);
	f1_full->SetLineColor(kRed);

	f1_el1->SetNpx(1000);
	f1_el2->SetNpx(1000);
	f1_el3->SetNpx(1000);
	f1_full->SetNpx(1000);
	f1_el1->Draw("same");
	f1_el2->Draw("same");
	f1_el3->Draw("same");
	f1_full->Draw("same");

	double Ndec = f1->GetParameter(0) /log(2) / (h1_dTdecTimp->GetXaxis()->GetBinWidth(1));
	//double Ndec = f1->GetParameter(0) * (f1->GetParameter(1)/log(2)) / (h1_dTdecTimp->GetXaxis()->GetBinWidth(1));
	double Nimp = ((TH2*)(file->Get("h1_Timp_target")))->GetEntries();

	fprintf(stdout, "entries in Timp: %.1f\n",Nimp);
	fprintf(stdout, "area of exp(amp*tau/binw): %.1f\n",Ndec);
	fprintf(stdout, "eff: %.3f\n", Ndec/Nimp);
	fprintf(stdout, "\n");

	double Nwhole = f1->Integral(0,2*f1->GetParameter(1)/log(2));
	TF1 *f1_bg = (TF1*) f1->Clone();
	f1_bg->SetParameter(0,0);
	double Nbg = f1_bg->Integral(0,2*f1->GetParameter(1)/log(2));

	fprintf(stdout, "integral Nwhole %.0f Nbg %.0f Nwhole-Nbg %.0f\n",Nwhole,Nbg,Nwhole-Nbg);
	fprintf(stdout, "p/b ratio: %f (~2tau)\n", (Nwhole-Nbg)/Nbg);

	TLatex latex;
	latex.SetTextSize(0.05);

	latex.DrawLatex(0.3,bg*1.05,
			Form("T_{1/2}=%.2f (%.0f) s", f1->GetParameter(1),f1->GetParError(1)*100 ));
	latex.DrawLatex(0.3,bg*0.8,
			Form("eff: %.3f (%.2e)\n", Ndec/Nimp, Nimp));
	latex.DrawLatex(0.3,bg*0.6,
			Form("p/b ratio: %.2f (~2tau)\n", (Nwhole-Nbg)/Nbg));


	/*c1->cd(3);
	TH3 *h3_Eg_dTgTdec_dTdecTimp = (TH3*) file->Get("h3_Eg_dTgTdec_dTdecTimp");
	double window_dTdecTimp = h3_Eg_dTgTdec_dTdecTimp->GetZaxis()->GetBinWidth(16);
	double window_dTgTdec = 5;
	h3_Eg_dTgTdec_dTdecTimp->RebinX(4);
	h3_Eg_dTgTdec_dTdecTimp->GetYaxis()->SetRangeUser(-window_dTgTdec,window_dTgTdec);
	
	h3_Eg_dTgTdec_dTdecTimp->GetZaxis()->SetRangeUser(-window_dTdecTimp,0);
	TH1D *h1_Eg_neg = (TH1D*) h3_Eg_dTgTdec_dTdecTimp->Project3D("x"); 
	h1_Eg_neg->SetName("h1_Eg_neg"); h1_Eg_neg->SetTitle("h1_Eg_neg"); h1_Eg_neg->Sumw2();
	h3_Eg_dTgTdec_dTdecTimp->GetZaxis()->SetRangeUser(0,window_dTdecTimp);
	TH1D *h1_Eg_pos = (TH1D*) h3_Eg_dTgTdec_dTdecTimp->Project3D("x"); 
	h1_Eg_pos->SetName("h1_Eg_pos"); h1_Eg_pos->SetTitle("h1_Eg_pos"); h1_Eg_pos->Sumw2();
	TH1D *h1_Eg_sub = (TH1D*) h1_Eg_pos->Clone();
	h1_Eg_sub->SetName("h1_Eg_sub"); h1_Eg_sub->SetTitle("h1_Eg_sub");
	h1_Eg_sub->Reset(); h1_Eg_sub->Add(h1_Eg_pos,1); h1_Eg_sub->Add(h1_Eg_neg,-1);

	fprintf(stdout, "bincontent %f %f %f\n", h1_Eg_pos->GetBinContent(30), h1_Eg_neg->GetBinContent(30), h1_Eg_sub->GetBinContent(30));
	fprintf(stdout, "binerror   %f %f %f\n", h1_Eg_pos->GetBinError(30), h1_Eg_neg->GetBinError(30), h1_Eg_sub->GetBinError(30));

	THStack *hs = new THStack("hs_Eg",Form("hs_Eg; Eg [keV]; count/%.0f keV", h3_Eg_dTgTdec_dTdecTimp->GetXaxis()->GetBinWidth(1)));
	h1_Eg_neg->SetLineColor(kRed  );	hs->Add(h1_Eg_neg);
	h1_Eg_pos->SetLineColor(kBlue );	hs->Add(h1_Eg_pos);
	h1_Eg_sub->SetLineColor(kGreen);	hs->Add(h1_Eg_sub);

	TLegend *lgd = new TLegend(0.7,0.7, 0.9,0.9);
	lgd->AddEntry(h1_Eg_pos);
	lgd->AddEntry(h1_Eg_neg);
	lgd->AddEntry(h1_Eg_sub);

	hs->Draw("nostack e");
	hs->GetXaxis()->SetRangeUser(200,400);
	lgd->Draw("same");*/



}
