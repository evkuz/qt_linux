#ifndef MCINFER_H
#define MCINFER_h

#include <float.h>
#include <stddef.h>
#include <QtMath>

using namespace std;

/***********************************************************************
			CONFIGURATION SECTION
***********************************************************************/

#ifndef FLT_EPSILON
#define FLT_EPSILON	0.0000001f   ///< minimal F.P value, typically defined in float.h
#endif

#define ZERO_LIMIT	(FLT_EPSILON*4.0f)	///< values less than specified will be considered equal 0

#define MAX_INPUT_VARS	5	///< maximal number of input variables
#define MAX_OUTPUT_VARS 5	///< maximal number of output variables
#define MAX_INPUT_MF	40	///< maximal number of MFs in all input variables
#define MAX_OUTPUT_MF	40	///< maximal number of MFs in all output variables

#define MAX_RULES		300	 ///< maximal number of rules in DB
#define MAX_DB_SIZE		8000 //1024 ///< maximal size of rule database in bytes

typedef unsigned int counter;		///< type used to count variables and MF's
typedef unsigned int rule_counter;	///< type used to count rules
typedef unsigned char mamdani_type;		///< type used to store mamdani rules

#define USE_EXP						///< define to enable exp() function and  normal distributions
#define USE_SUGENO_0				///< define to enable sugeno-0 code
#define USE_SUGENO_1				///< define to enable sugeno-1 code
#define USE_MAMDANI					///< define to enable mamdani code

/***********************************************************************
			DATA TYPES
***********************************************************************/

#define EC_SUGENO_0	1	///< Sugeno order 0
#define EC_SUGENO_1 2	///< Sugeno order 1
#define EC_MAMDANI	3	///< Mamdani

#define DB_COMPLETE 1	///< complete rule database
#define DB_LBRW		2	///< LBRW rule database

typedef struct _scale_data
{
	float _scale;
	float _offset;
} scale_data;

typedef enum { exact, ///< non-fuzzy number
triangular,	///< triangular distribution (_modal_value,_left_fuzzy,_right_fuzzy).
trapezium, ///< trapezium distribution (_left_tolerant,_right_tolerant,_left_fuzzy,_right_fuzzy).
ascending, ///< ascending distribution (_modal_value, _left_fuzzy)
descending ///< descending distribution (_modal_value, _right_fuzzy)
#ifdef USE_EXP
, 
normal,		///< symmetrical normal distribution (_modal_value,_right_fuzzy)
asym_normal,///< assymetrical normal distribution (_modal_value,_left_fuzzy,_right_fuzzy)
asc_normal,	///< ascending normal distribution (_modal_value, _left_fuzzy)
desc_normal ///< descending normal distribution (_modal_value, _right_fuzzy)
#endif
} FDistr;

typedef struct _mf_data {
	FDistr _distribution;
	float _lefttolerant;	///< also modal value
	float _righttolerant;
	float _leftfuzzy;
	float _rightfuzzy;
} mf_data;
#define _modalvalue _lefttolerant


//! Main fuzzy model configuration data and temporary array

typedef struct {

	unsigned char model_ready;			///< 0 if model can be used for calculations, non-zero if model is being reconfigured
	unsigned char inference_algorithm;	///< inference algorithm: EC_SUGENO_0, EC_SUGENO_1 or EC_MAMDANI
	unsigned char min_inference_mode;	///< if TRUE min is used for inference, FALSE otherwise
	unsigned char db_type;				///< rule database type: DB_COMPLETE or DB_LBRW

	counter num_input_vars;			///< number of input variables
	counter num_output_vars;		///< number of output variables

	const scale_data input_scales[MAX_INPUT_VARS];
	const scale_data output_scales[MAX_OUTPUT_VARS];

	counter in_mf_num[MAX_INPUT_VARS];	///< number of MF's per input variable
	mf_data in_mf[MAX_INPUT_MF];	///< input MF distributions

	counter out_mf_num[MAX_OUTPUT_VARS]; ///< number of MF's per output variable
	mf_data out_mf[MAX_OUTPUT_MF]; ///< output MF distributions

	rule_counter num_rules;					///< Number of rules in DB
	rule_counter db_index[MAX_RULES];		///< LBRW rule database index
	unsigned char db_data[MAX_DB_SIZE];		///< rule database

	float input_data[MAX_INPUT_VARS];		///< array with non-normalized input values

	union _algorithm_data
	{
#ifdef USE_SUGENO_0
		struct _sugeno_0
		{
			float up[MAX_OUTPUT_VARS];
			float down[MAX_OUTPUT_VARS];
		} sugeno_0;
#endif
#ifdef USE_SUGENO_1
		struct _sugeno_1
		{
			float up[MAX_OUTPUT_VARS];
			float down[MAX_OUTPUT_VARS];
		} sugeno_1;
#endif
#ifdef USE_MAMDANI
		struct _mamdani
		{
			float level[MAX_OUTPUT_MF];
		} mamdani;
#endif
	} algorithm_data;
} fuzzy_model;


/***********************************************************************
			Functions
***********************************************************************/

extern "C" {

  
//! Run inference
/*!
\param model pointer to fuzzy model data structure
\param input pointer to array of input data x 1000
\param output pointer to array of output data x 1000
\returns number of activated rules, negative value indicate error
*/
int MCInfer(fuzzy_model *model,float *input,float *output);	

//! MCinfer return codes
#define MODEL_NOT_READY	-1	///< model ready flag is not set
#define BAD_DB_TYPE	-3		///< unknown database type
#define BAD_ALGORITHM	-4	///< unknown inference algorithm
#define NO_RESULT	-5		///< GetResult() failed

//! Prepare model for inference
/*!
Initializes algorithm data
\param model pointer to fuzzy model data structure
*/
void Init(fuzzy_model *model);	

//! Calculate MF level
/*!
\param mf pointer to MF parameter data
\param x input data (normalized)
\retruns level of MF activation
*/
float GetLevel(const mf_data *mf,float x);

//! Find and run activated rules
/*!
\param model pointer to fuzzy model data structure
\param mf_level array of MF levels
\return number of activated rules, negative value on error
*/
int EnumRules(fuzzy_model *model,float *mf_level);

//! Activate single rule
/*!
\param model pointer to fuzzy model data structure
\param level rule activation level
\param rule pointer to rule data
\returns \a 1 on success, \a 0 if inference algorith is unknown
*/
int ActivateRule(fuzzy_model *model,float level,const void *rule);

//! Get result and clear data for next run
/*!
\param model pointer to fuzzy model data structure
\param output addres of destination variables
\returns \a TRUE on success, \a FALSE if output can't be computed
*/
int GetResult(fuzzy_model *model,float *output);

//! Returns length of single rule
/*!
\param model pointer to fuzzy model data structure
\returns memory size in bytes required to store single rule (for single output)
*/
size_t RuleSize(fuzzy_model *model);
}
#endif //
