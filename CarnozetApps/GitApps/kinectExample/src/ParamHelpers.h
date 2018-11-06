//
//  ParamHelpers.h
//  kinectExample
//
//  Created by Martin Hermant on 06/11/2018.
//
//

#ifndef ParamHelpers_h
#define ParamHelpers_h

#define P(nom,init,min,max) nom.setName(#nom);nom.setMin(min);nom.setMax(max);settings.add(nom);nom=init;
#define PB(nom,init) P(nom,init,false,true);


#endif /* ParamHelpers_h */
