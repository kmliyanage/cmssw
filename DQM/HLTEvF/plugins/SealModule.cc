// $Id: SealModule.cc,v 1.12 2008/08/06 12:00:31 bjbloom Exp $

#include "DQM/HLTEvF/interface/PathTimerService.h"
#include "DQM/HLTEvF/interface/PathTimerInserter.h"
#include "DQM/HLTEvF/interface/HLTTauDQMSource.h"
#include "DQM/HLTEvF/interface/HLTMonElectron.h"
#include "DQM/HLTEvF/interface/HLTMonElectronConsumer.h"
#include "DQM/HLTEvF/interface/FourVectorHLT.h"
#include "DQM/HLTEvF/interface/HLTMon.h"
#include "DQM/HLTEvF/interface/HLTEventInfoClient.h"

using edm::service::PathTimerService;

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
DEFINE_FWK_SERVICE(PathTimerService);
//DEFINE_FWK_SERVICE_MAKER(PathTimerService,PathTimerServiceMaker);

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PathTimerInserter);
DEFINE_FWK_MODULE(HLTTauDQMSource);
DEFINE_FWK_MODULE(HLTMonElectron);
DEFINE_FWK_MODULE(HLTMonElectronConsumer);
DEFINE_FWK_MODULE(HLTEventInfoClient);
DEFINE_FWK_MODULE(FourVectorHLT);
DEFINE_FWK_MODULE(HLTMon);

