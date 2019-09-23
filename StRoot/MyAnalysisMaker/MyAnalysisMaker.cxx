#include "MyAnalysisMaker.h"
#include <iostream>

#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StBTofHeader.h"
#include "StMessMgr.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TObjArray.h"
#include "TComplex.h"
#include "nsmEvent.h"
#include "nsmTrack.h"
#include "TClonesArray.h"
#include "TTree.h"



using namespace std;

ClassImp(MyAnalysisMaker)                       // Macro for CINT compatibility
MyAnalysisMaker::MyAnalysisMaker(StMuDstMaker* maker ) : StMaker("MyAnalysisMaker")
{                                               // Initialize data members here.
    mMuDstMaker      = maker ;                    // Pass MuDst pointer to DstAnlysisMaker Class member functions
    histogram_output = NULL  ;                    // Zero the Pointer to histogram output file
    mEventsProcessed = 0     ;                    // Zero the Number of Events processed by the maker
    OutputFileName = "" ;                         // Output File Name( will be set inside the "readMuDst".C )
    
}


MyAnalysisMaker::~MyAnalysisMaker()
{/* */}


Int_t MyAnalysisMaker::Init()
{
  //    cout << "I did initialize" << endl;
    //----------------------------------------
    runnumber        =      -999;  //just a no.
    Pi               =   3.14159;
    twoPi            =   6.28318;
    
    
    //-----------------------------------------------------------------------------------------
    histogram_output = new TFile(OutputFileName,"RECREATE") ;  //
    
    nsmTree = new TTree("nsmTree","nsmTree");//                                                                                                           
    protonArr = new TClonesArray("nsmTrack", 1000); //                                                                                                  
    nsmTree->Branch("Proton", &protonArr, 256000, 99);//                                                                                                
    levent = new nsmEvent();//                                                                                                                          
    nsmTree->Branch("Event", "nsmEvent", &levent, 256000, 99);//   
    
    VertexZPos = -100.0;
    VpdVzPos   = -100.0;
    
    return kStOK ;
}



Bool_t MyAnalysisMaker::IsBadEvent(StMuEvent *muEvent)
{
    
    if(!muEvent) return kTRUE;
    
    double vx = muEvent->primaryVertexPosition().x();
    double vy = muEvent->primaryVertexPosition().y();
    double vz = muEvent->primaryVertexPosition().z();
    if( (vx < 1.e-5 && vx > -1.e-5) &&
       (vy < 1.e-5 && vy > -1.e-5) &&
       (vz < 1.e-5 && vz > -1.e-5)  )
        return kTRUE;
    
    //Vpd-Zdc-mb & Vpd-Zdc-mb-protected  AuAu2011
  /*  if(!(muEvent->triggerIdCollection().nominal().isTrigger(350001) || muEvent->triggerIdCollection().nominal().isTrigger(350011) || muEvent->triggerIdCollection().nominal().isTrigger(350003) || muEvent->triggerIdCollection().nominal().isTrigger(350013) || muEvent->triggerIdCollection().nominal().isTrigger(350023) || muEvent->triggerIdCollection().nominal().isTrigger(350033) || muEvent->triggerIdCollection().nominal().isTrigger(350043)))
        return kTRUE;
    
    //mb AuAu62
    if(!(muEvent->triggerIdCollection().nominal().isTrigger(270001) || muEvent->triggerIdCollection().nominal().isTrigger(270011) || muEvent->triggerIdCollection().nominal().isTrigger(270021)))
        return kTRUE;
  */
    //mb AuAu39
    //if(!(muEvent->triggerIdCollection().nominal().isTrigger(280001)))
    //return kTRUE;
   
    //mb AuAu27
    //if(!(muEvent->triggerIdCollection().nominal().isTrigger(360001)))
    //return kTRUE;
   
    //mb AuAu19
    //if(!(muEvent->triggerIdCollection().nominal().isTrigger(340001) || muEvent->triggerIdCollection().nominal().isTrigger(340011) || muEvent->triggerIdCollection().nominal().isTrigger(340021)))
    //return kTRUE;
   
    //mb AuAu14
    //if(!(muEvent->triggerIdCollection().nominal().isTrigger(440001) || muEvent->triggerIdCollection().nominal().isTrigger(440004) || muEvent->triggerIdCollection().nominal().isTrigger(440005)))
    //return kTRUE;
    
    //mb AuAu11
    if(!(muEvent->triggerIdCollection().nominal().isTrigger(310004) || muEvent->triggerIdCollection().nominal().isTrigger(310014)))
    return kTRUE;
   
    //mb AuAu7
    //if(!(muEvent->triggerIdCollection().nominal().isTrigger(290004)))
    //return kTRUE;
    
    
    if(fabs(vz)>30.0) // change to 50 for 7 GeV
        return kTRUE;
    
    if(sqrt(vx*vx+vy*vy)>2.0) 
      //if(sqrt(pow(vx,2.)+pow((vy+0.89),2.))>1.) //for 14 GeV
        return kTRUE;
    
    return kFALSE;
}


Int_t MyAnalysisMaker::Make()
{
 
    StMuEvent* muEvent  =  mMuDstMaker->muDst()->event();
    
    runnumber = muEvent->runId();
    
    int good = 1;
    
    int BadRunList[1000] = {11114085, 11114089, 11114095, 11115005, 11115042, 11115064, 11115080, 11116027, 11116028, 11116060, 11116068, 11116070, 11117044, 11117075, 11117088, 11117090, 11117098, 11118007, 11118008, 11118015, 11119024, 11119062, 11120040, 11120042, 11121044, 11121054, 11122037, 11122049, 11122050, 11122073, 11123028, 11123051, 11123053, 11123054, 11123055, 11123056, 11123057, 11123058, 11123059, 11123060, 11123076, 11123095, 11123102, 11124041, 11124046, 11124047, 11124048, 11124050, 11124062, 11128056, 11129028, 11133006, 11136005, 11136012, 11136013, 11142117, 11147006, 11148001, 11148045, 11149015, 11149017, 11149018, 11149047, 11150025, 11150029, 11151050, 11151057, 11152016, 11152036, 11152078, 11153032, 11154026, 11156009, 11156036, 11156043, 11156044, 11156045, 11157050, 11158011, 11158012, 11158015, 11158016, 11158017, 11158019, 11158020, 11158021, 11158022, 11158024, 15046096, 15046106, 15046107, 15046108, 15046110, 15046111, 15047004, 15047015, 15047019, 15047021, 15047023, 15047027, 15047028, 15047029, 15047030, 15047039, 15047041, 15047044, 15047050, 15047056, 15047057, 15047061, 15047062, 15047064, 15047068, 15047069, 15047071, 15047074, 15047075, 15047082, 15047093, 15051131, 15051132, 15053052, 15053053, 15053054, 15053055, 12113091, 12114007, 12115014, 12115015, 12115016, 12115018, 12115019, 12115020, 12115022, 12115023, 12115026, 12115073, 12116012, 12116015, 12116016, 12120026, 12121017, 12121022, 12121034, 12122019, 12172050, 12172051, 12172055, 12173009, 12173030, 12173031, 12173032, 12173033, 12173034, 12174077, 12174085, 12174096, 12175062, 12175087, 12175113, 12175114, 12175115, 12176054, 12176067, 12176069, 12177092, 12177099, 12177101, 12177106, 12177107, 12177108, 12178003, 12178004, 12178005, 12178006, 12178051, 12178099, 12178123, 12179068, 11099124, 11100034, 11100045, 11101046, 11101104, 11102012, 11102098, 11103008, 11103009, 11103046, 11103058, 11103062, 11103065, 11105011, 11105018, 11105029, 11106026, 11106027, 11106028, 11106029, 11106030, 11106040, 11106041, 11107061, 11108101, 11109013, 11109088, 11109090, 11109092, 11109097, 11109098, 11109100, 11109101, 11109102, 11109103, 11109104, 11109105, 11110041, 11110073, 11084009, 11085015, 11087001, 11087002, 11087003, 11087004, 11087057, 11087058, 11087059, 11089026, 11089048, 11089055, 11092012, 11092031, 11092032, 11092033, 11092034, 11095022, 11095048, 11095050, 11095051, 11095061, 11095062, 11095063, 11095064, 11002145, 11003001, 11003002, 11003006, 11003012, 11003013, 11023046, 11023048, 11025034, 11025054, 11028004, 11028005, 11028006, 11028018, 11028019, 11028020, 11028021, 11028022, 11057035, 11064003, 11070051, 11070052, 11070053, 11070054, 11070055, 11070056, 11070057, 11071001, 11071002, 11071003, 11071004, 11071006, 11071007, 11071013, 11071014, 11071015, 11071016, 11071017, 11071018, 11071019, 11071020, 11072030, 11072031, 11072032, 11072044, 11072045, 11073001, 11073003, 0, 0
    };
    
    for(int iii=0; iii<1000; iii++)
    {
        if(BadRunList[iii]==0) break; //end of bad run list
        if(runnumber==BadRunList[iii]) good = 2;
        
    }

    if(good == 2) return kStOK;
    
    
    if(IsBadEvent(muEvent))                                       //Nominal Event cuts and trigger cut
        return           kStOK;
    cout << "good run" << endl;    
    //----------------------------------------------------
    VertexZPos  =  muEvent-> primaryVertexPosition().z();
    VpdVzPos    =  muEvent-> vpdVz();
   
    //if(fabs(VpdVzPos-VertexZPos) < 3) return kStOK; // for 39 GeV +
    
    //---------------------------------------------------------
    
    
    int nHitsFit, check, nHitsDedx;
    float ratio, dca, eta, pt, nsigmapr, phi, charge, Qx, Qy;
    double beta, p;

    int protonp = 0;
    refmult2 = 0;
    Qx = 0; Qy = 0;
    
    TObjArray* tracks = mMuDstMaker->muDst()->primaryTracks() ;    // Create a TObject array containing the primary tracks
    TObjArrayIter  GetTracks(tracks) ;                              // Create an iterator to step through the tracks
    StMuTrack*                 track ;                              // Pointer to a track

    while((track = (StMuTrack*)GetTracks.Next()))
    {
        check = 0;
        
        nHitsFit =  track->nHitsFit();
        nHitsFit =  fabs(nHitsFit)+1;
        ratio    =  (float) nHitsFit / (float) track->nHitsPoss();
        if(ratio  < 0.52) continue;
        if(ratio > 1.05) continue;
                
        nHitsDedx = track->nHitsDedx();
        dca = track->dcaGlobal().mag();
        eta = track->eta();
        
        phi = track->phi();
        if(phi < 0) phi = phi + twoPi;
        
        charge = track->charge();
        if(fabs(charge)!=1) continue;
        
        p = track->p().mag();
        if (p < 0.15) continue;
        
        pt = track->pt();
        
        if(nHitsFit > 10 && dca < 3.0 && fabs(eta) > 0.5 && fabs(eta) < 1.0) refmult2++;
        
        if(nHitsFit > 15 && dca < 2.0 && fabs(eta) < 1.0 && pt > 0.2 && pt < 2.0) {Qx = Qx + cos(2*phi); Qy = Qy + sin(2*phi);}
        
	if(nHitsFit < 20) continue;
	if(nHitsDedx <= 5) continue;

        nsigmapr = track->nSigmaProton();
        if(fabs(nsigmapr) > 2.2) continue; // < 1 for 27 GeV
        
        if(fabs(eta) > 0.6) continue;
        if(dca < 0 || dca > 2.2) continue;

        if(pt < 0.3) continue;
        if(pt > 2.5) continue;
        
        beta = -999;
        beta = track->btofPidTraits().beta();
        
        new((*protonArr)[protonp++]) nsmTrack(pt,p,phi,eta,dca,nsigmapr,beta,charge);

    }//==================track loop ends=========================

    TVector2 Q(Qx,Qy);
    double EventPlane = 0.5 * Q.Phi();
    Qx = 0;
    Qy = 0;
    
    levent->SetEventData(muEvent->primaryVertexPosition().x(), muEvent->primaryVertexPosition().y(), muEvent->primaryVertexPosition().z(), muEvent->refMult(), runnumber, refmult2, EventPlane, muEvent->btofTrayMultiplicity());
    
    //fill tree
    nsmTree->Fill();
    protonArr->Delete();

    cout << "found " << endl; 
    mEventsProcessed++ ;
    return kStOK ;
    
}//--------------- Make (Event) loop ends--------------------------


Int_t MyAnalysisMaker::Finish()
{
    cout<<" Inside Finish and writing histograms..."<<endl;
    cout << endl;
    cout << endl;
    
    histogram_output -> Write();
    histogram_output ->Close();
    
    cout <<"\n ======> All done <======"<<endl;
    cout<<" Acutal #Events Processed = " <<mEventsProcessed<<"\n###### Thank You ######\n"<< endl ;
    
    return kStOk ;
}
