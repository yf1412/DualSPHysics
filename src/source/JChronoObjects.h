/*
 <DUALSPHYSICS>  Copyright (c) 2016, Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

 EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
 School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

 This file is part of DualSPHysics. 

 DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

 DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. 

 You should have received a copy of the GNU General Public License, along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

//#############################################################################
//# Cambios:
//# =========
//# - Hace de interface con la libreria dsphchrono.dll. (03-05-2016)
//# - Permite compilar sin librerias de CHRONO. (13-12-2019)
//#############################################################################

/// \file JChronoObjects.h \brief Declares the class \ref JChronoObjects.

#ifndef _JChronoObjects_
#define _JChronoObjects_

#include "JObject.h"
#include "DualSphDef.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

//#define DISABLE_CHRONO     ///<It allows compile without ChronoLib library.

class JLog2;
class JSphMk;
class JXml;
class TiXmlElement;
class JChronoData;
class JChValues;
class JChBody;
class JChLink;
class DSPHChronoLib;
class JChBodyFloating;

//##############################################################################
//# JChronoObjects
//##############################################################################
/// \brief Manages Chrono objects.

#ifdef DISABLE_CHRONO
#include "JChronoObjectsUndef.h"
#else
class JChronoObjects : protected JObject
{
protected:
	unsigned Solver; 
  int OmpThreads;        ///<Max number of OpenMP threads in execution on CPU host (minimum 1). | Numero maximo de hilos OpenMP en ejecucion por host en CPU (minimo 1). <chrono_parallel>
  JLog2 *Log;
  std::string DirData;
  std::string CaseName;
  const double Dp;
  const word MkBoundFirst;
  const bool UseDVI;	///<Uses Differential Variational Inequality (DVI) method.
	bool UseChronoSMC;		///<Uses DVI method and extra variables (Young's Modulus and Poisson Ratio).
  
  bool WithMotion;   ///<Some Chrono object with geometry is a moving object.
  
  void* Ptr_VtkSimple_AutoActual;
  void* Ptr_VtkSimple_AutoDp;

  JChronoData *ChronoDataXml; ///<Datos de Chrono cargados del XML.
  
  DSPHChronoLib *ChronoLib;   ///<Objeto para integracion con libreria de Chrono Engine.

  float CollisionDp;   ///<Allowed collision overlap according Dp (default=0.5).
  double SchemeScale;  ///<Scale value to create initial scheme of configuration.

  double SaveDataTime;  ///<Saves CSV with data exchange (0=all steps, <0:none).
  double NextTime;
  double LastTimeOk;

  void LoadPtrAutoActual(const JXml *sxml,std::string xmlrow);
  void LoadPtrAutoDp(const JXml *sxml,std::string xmlrow);
  
  void CreateObjFiles(std::string idname,const std::vector<unsigned> &mkbounds
    ,std::string datadir,std::string mfile,byte normalmode,std::string diroutobj,std::string xmlrow);

  void LoadXml(JXml *sxml, const std::string &place);
  void ReadXml(const JXml *sxml, TiXmlElement* lis);
  void ReadXmlValues(const JXml *sxml,TiXmlElement* lis,JChValues* values);
  std::string ReadXmlModelFile(const JXml *sxml,TiXmlElement* ele)const;
  void ConfigMovingBodies(const JSphMk* mkinfo);

  void VisuValues(const JChValues *values)const;
  void VisuBody(const JChBody *body)const;
  void VisuLink(const JChLink *link)const;
  void SaveVtkScheme()const;
	void ConfigOmp();

public:
  JChronoObjects(JLog2* log,const std::string &dirdata,const std::string &casename
    ,JXml *sxml,const std::string &place,double dp,word mkboundfirst);
  ~JChronoObjects();
  void Reset();
  static bool Available(){ return(true); }

  bool UseDataDVI(word mkbound)const;
  bool ConfigBodyFloating(word mkbound,double mass,const tdouble3 &center
    ,const tmatrix3d &inertia,const tint3 &translationfree,const tint3 &rotationfree
    ,const tfloat3 &linvelini,const tfloat3 &angvelini);
  void ConfigDataDVIBodyFloating(word mkbound,float kfric,float restitu); 
  void ConfigDataDVIBodyMoving  (word mkbound,float kfric,float restitu);	
  void ConfigDataDVIBodyFixed   (word mkbound,float kfric,float restitu);	

	void ConfigDataBodyFloating(word mkbound,float kfric,float restitu,float young=0.0,float poisson=0.0);//<chrono_contacts>
	void ConfigDataBodyMoving  (word mkbound,float kfric,float restitu,float young=0.0,float poisson=0.0);//<chrono_contacts>
	void ConfigDataBodyFixed   (word mkbound,float kfric,float restitu,float young=0.0,float poisson=0.0);//<chrono_contacts>

  void Init(bool simulate2d,const JSphMk* mkinfo);
  void VisuConfig(std::string txhead, std::string txfoot)const;

  bool GetWithMotion()const{ return(WithMotion); }

  void SetFtData(word mkbound,const tfloat3 &face,const tfloat3 &fomegaace);
  void SetFtDataVel(word mkbound,const tfloat3 &vlin,const tfloat3 &vang); //<vs_fttvel>
  void GetFtData(word mkbound,tdouble3 &fcenter,tfloat3 &fvel,tfloat3 &fomega)const;

  void SetMovingData(word mkbound,bool simple,const tdouble3 &msimple,const tmatrix4d &mmatrix,double stepdt);

  void RunChrono(unsigned nstep,double timestep,double dt,bool predictor);

  void SavePart(int part);

	//JChBodyFloating* ReadElementFEA(const JXml *sxml,TiXmlElement* ele,unsigned idb,const std::string idname,const word mkbound);

};
#endif

#endif

