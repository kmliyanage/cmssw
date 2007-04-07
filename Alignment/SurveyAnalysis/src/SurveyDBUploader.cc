#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/CommonAlignment/interface/SurveyDet.h"
#include "Alignment/SurveyAnalysis/interface/SurveyInputBase.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/Alignment/interface/SurveyErrors.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Alignment/SurveyAnalysis/interface/SurveyDBUploader.h"

SurveyDBUploader::SurveyDBUploader(const edm::ParameterSet& cfg):
  theValueTag( cfg.getParameter<std::string>("valueTag") ),
  theErrorTag( cfg.getParameter<std::string>("errorTag") ),
  theValues(0),
  theErrors(0)
{
}

void SurveyDBUploader::endJob()
{
  theValues = new SurveyValues;
  theErrors = new SurveyErrors;

  theValues->m_align.reserve(65536);
  theErrors->m_surveyErrors.reserve(65536);

  getSurveyInfo( SurveyInputBase::detector() );

  edm::Service<cond::service::PoolDBOutputService> poolDbService;

  if( poolDbService.isAvailable() )
  {
    if ( poolDbService->isNewTagRequest(theValueTag) )
      poolDbService->createNewIOV<SurveyValues>
	(theValues, poolDbService->endOfTime(), theValueTag);
    else
      poolDbService->appendSinceTime<SurveyValues>
	(theValues, poolDbService->currentTime(), theValueTag);

    if ( poolDbService->isNewTagRequest(theErrorTag) )
      poolDbService->createNewIOV<SurveyErrors>
	(theErrors, poolDbService->endOfTime(), theErrorTag);
    else
      poolDbService->appendSinceTime<SurveyErrors>
	(theErrors, poolDbService->currentTime(), theErrorTag);
  }
  else
    throw cms::Exception("ConfigError")
      << "PoolDBOutputService is not available";
}

void SurveyDBUploader::getSurveyInfo(const Alignable* ali)
{
  const std::vector<Alignable*>& comp = ali->components();

  unsigned int nComp = comp.size();

  for (unsigned int i = 0; i < nComp; ++i) getSurveyInfo(comp[i]);

  const SurveyDet* survey = ali->survey();

  const align::PositionType& pos = survey->position();
  const align::RotationType& rot = survey->rotation();

  SurveyValue value( CLHEP::Hep3Vector( pos.x(), pos.y(), pos.z() ),
		     CLHEP::HepRotation
		     ( CLHEP::HepRep3x3( rot.xx(), rot.xy(), rot.xz(),
					 rot.yx(), rot.yy(), rot.yz(),
					 rot.zx(), rot.zy(), rot.zz() ) ),
		     ali->geomDetId().rawId() );

  SurveyError error( ali->alignableObjectId(),
		     ali->geomDetId().rawId(),
		     survey->errors() );

  theValues->m_align.push_back(value);
  theErrors->m_surveyErrors.push_back(error);
}
