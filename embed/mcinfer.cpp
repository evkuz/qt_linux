#include "mcinfer.h"
//#include "mainwindow.h"

#ifdef USE_EXP
#include <math.h>
#endif

// version 13.10.2012

float input_levels[MAX_INPUT_MF];
float out[MAX_OUTPUT_VARS];
counter index2[MAX_INPUT_VARS]={0,0,0,0,0};


int MCInfer(fuzzy_model *model,float *input, float *output)
{
	counter i,j,k;
	int res=0;

	if(model->model_ready) return MODEL_NOT_READY;

	Init(model);

	// calculate input MF levels

	for(i=0,k=0;i<model->num_input_vars;i++)
	{
		model->input_data[i]=((float)input[i]);
		for(j=0;j<model->in_mf_num[i];j++,k++)
		{
			input_levels[k]=GetLevel(&model->in_mf[k],
				model->input_data[i]*model->input_scales[i]._scale+model->input_scales[i]._offset);
		}
	}

	// search for rules and run inference

	res=EnumRules(model,input_levels);
	if(res<=0) return res;

	if(!GetResult(model,out)) return NO_RESULT;

	// denormalize output

	for(i=0;i<model->num_output_vars;i++)
		output[i]=(out[i]-model->output_scales[i]._offset)/model->output_scales[i]._scale;

	return res;
}

float GetLevel(const mf_data *mf,float x)
{
	float res=0.0f;

	switch(mf->_distribution)
	{
		case exact:
			if(mf->_modalvalue>ZERO_LIMIT) // compare non-zero float values
			{
				if( x/mf->_modalvalue<1+FLT_EPSILON*2.0f &&
					x/mf->_modalvalue>1-FLT_EPSILON*2.0f)
				res=1.0f;
			}
			else // compare float value with zero
			{
				if(mf->_modalvalue-ZERO_LIMIT<x &&
				   mf->_modalvalue+ZERO_LIMIT>x)
				   res=1.0f;
			}
			break;

		case triangular:
			if(x>mf->_modalvalue-mf->_leftfuzzy && x<=mf->_modalvalue)
				res=(x-mf->_modalvalue+mf->_leftfuzzy)/mf->_leftfuzzy;
			else if(x>=mf->_modalvalue && x<mf->_modalvalue+mf->_rightfuzzy)
				res=(mf->_modalvalue+mf->_rightfuzzy-x)/mf->_rightfuzzy;
			break;

		case trapezium:
			if(x>mf->_lefttolerant-mf->_leftfuzzy && x<=mf->_lefttolerant)
				res=(x-mf->_lefttolerant+mf->_leftfuzzy)/mf->_leftfuzzy;
			else if(x>=mf->_lefttolerant && x<=mf->_righttolerant)
				res=1.0f;
			else if(x>=mf->_righttolerant && x<mf->_righttolerant+mf->_rightfuzzy)
				res=(mf->_righttolerant+mf->_rightfuzzy-x)/mf->_rightfuzzy;
			break;

		case ascending:
			if(x>mf->_modalvalue-mf->_leftfuzzy && x<=mf->_modalvalue)
				res=(x-mf->_modalvalue+mf->_leftfuzzy)/mf->_leftfuzzy;
			else if(x>=mf->_modalvalue)
				res=1.0f;
			break;

		case descending:
			if(x>=mf->_modalvalue && x<mf->_modalvalue+mf->_rightfuzzy)
				res=(mf->_modalvalue+mf->_rightfuzzy-x)/mf->_rightfuzzy;
			else if(x<=mf->_modalvalue)
				res=1.0f;
			break;

#ifdef USE_EXP
		case normal:
			res=(float)exp(-9.0f*(x-mf->_modalvalue)*(x-mf->_modalvalue)/(2.0f*mf->_rightfuzzy*mf->_rightfuzzy));
			break;

		case asym_normal:
			if(x>=mf->_modalvalue)
				res=(float)exp(-9.0f*(x-mf->_modalvalue)*(x-mf->_modalvalue)/(2.0f*mf->_rightfuzzy*mf->_rightfuzzy));
			else
				res=(float)exp(-9.0f*(x-mf->_modalvalue)*(x-mf->_modalvalue)/(2.0f*mf->_leftfuzzy*mf->_leftfuzzy));
			break;

		case asc_normal:
			if(x>=mf->_modalvalue)
				res=1.0f;
			else
				res=(float)exp(-9.0f*(x-mf->_modalvalue)*(x-mf->_modalvalue)/(2.0f*mf->_leftfuzzy*mf->_leftfuzzy));
			break;

		case desc_normal:
			if(x>=mf->_modalvalue)
				res=(float)exp(-9.0f*(x-mf->_modalvalue)*(x-mf->_modalvalue)/(2.0f*mf->_rightfuzzy*mf->_rightfuzzy));
			else
				res=1.0f;
			break;
#endif

	}
	return res;
}


void Num2Index(fuzzy_model *model,rule_counter num,counter *idx,int extra)
{
  int i;

  if(idx)
	for(i=(int)model->num_input_vars-1;i>=0;i--)
	{
		idx[i]=num%(model->in_mf_num[i]+extra);
		num=num/(model->in_mf_num[i]+extra);
	}
}

int EnumRules(fuzzy_model *model,float *mf_level)
{
 int res=0;
 rule_counter r;
 float level;
 counter i,j;

 if(model->db_type==DB_COMPLETE)		/// complete database
 {
	for(r=0;r<model->num_rules;r++)
	{
		Num2Index(model,r,index2,0);
		level=1.0f;
		for(i=0,j=0;i<model->num_input_vars;i++)
		{
			if(model->min_inference_mode)
			{
				if(mf_level[j+index2[i]]<level) level=mf_level[j+index2[i]];
			}
			else
				level*=mf_level[j+index2[i]];
			j+=model->in_mf_num[i];
		}

		if(level>ZERO_LIMIT)
		{
			if(!ActivateRule(model,level,&model->db_data[r*model->num_output_vars*RuleSize(model)]))
				return BAD_ALGORITHM;
			res++;
		}
	}
 }
 else if(model->db_type==DB_LBRW)		// LBRW rule base
 {
	for(r=0;r<model->num_rules;r++)
	{
		Num2Index(model,model->db_index[r],index2,1);
		level=1.0f;
		for(i=0,j=0;i<model->num_input_vars;i++)
		{
			if(index2[i]>model->in_mf_num[i])
			{
					j+=model->in_mf_num[i];
					continue;
			}
			if(model->min_inference_mode)
			{
				if(mf_level[j+index2[i]]<level) level=mf_level[j+index2[i]];
			}
			else
				level*=mf_level[j+index2[i]];
			j+=model->in_mf_num[i];
		}

		if(level>ZERO_LIMIT)
		{
			if(!ActivateRule(model,level,&model->db_data[r*model->num_output_vars*RuleSize(model)]))
				return BAD_ALGORITHM;
			res++;
		}
	}
 }
 else	// DB type invalid
	return BAD_DB_TYPE;

 return res;
}


float SupportMinus(const mf_data *mf)
{
	if(mf->_distribution==descending
#ifdef USE_EXP
		||mf->_distribution==desc_normal
#endif
		)
			return -FLT_MAX;

	if(mf->_distribution==trapezium)
			return mf->_lefttolerant-mf->_leftfuzzy;

	return mf->_modalvalue-mf->_leftfuzzy;
}

float SupportPlus(const mf_data *mf)
{
	if(mf->_distribution==ascending
#ifdef USE_EXP
		||mf->_distribution==asc_normal
#endif
		)
			return FLT_MAX;

	if(mf->_distribution==trapezium)
			return mf->_righttolerant+mf->_rightfuzzy;

	return mf->_modalvalue+mf->_rightfuzzy;

}

//float min(float a, float b)
//{
//	if(a>b) return b;
//	else return a;
//}

int ActivateRule(fuzzy_model *model,float level,const void *rule)
{
	counter i,j;
	float z=0;

#if defined(USE_SUGENO_0) || defined(USE_SUGENO_1)
	float *f_rule=(float*)rule;
#endif

#ifdef USE_MAMDANI
	mamdani_type *m_rule=(mamdani_type*)rule;
#endif

	switch(model->inference_algorithm)
	{
#ifdef USE_SUGENO_0
	case EC_SUGENO_0:
		for(i=0;i<model->num_output_vars;i++)
		{
			model->algorithm_data.sugeno_0.up[i]+=level*f_rule[i];
			model->algorithm_data.sugeno_0.down[i]+=level;
		}
		break;
#endif

#ifdef USE_SUGENO_1
	case EC_SUGENO_1:
		for(i=0;i<model->num_output_vars;i++,f_rule++)
		{
			for(j=0;j<model->num_input_vars;j++,f_rule++)
			{
				z+=model->input_data[j]*(*f_rule);
			}
			model->algorithm_data.sugeno_1.up[i]+=level*(f_rule[0]);
			model->algorithm_data.sugeno_1.down[i]+=level;
		}
		break;
#endif

#ifdef USE_MAMDANI
	case EC_MAMDANI:
		for(i=0,j=0;i<model->num_output_vars;i++)
		{
			if(model->algorithm_data.mamdani.level[j+m_rule[i]]<level)
					model->algorithm_data.mamdani.level[j+m_rule[i]]=level;
			j+=model->out_mf_num[i];
		}
		break;
#endif
	default:
		return 0;
	}
	return 1;
}

int GetResult(fuzzy_model *model,float *output)
{
	counter i;
	int bad=0;

#ifdef USE_MAMDANI
	counter j,idx,start;
	float a,b,x;
	float c,n,t;
#endif


	switch(model->inference_algorithm)
	{
#ifdef USE_SUGENO_0
	case EC_SUGENO_0:
        for(i=0;i<model->num_output_vars;i++)
		{
			if(model->algorithm_data.sugeno_0.down[i]>ZERO_LIMIT)
				output[i]=model->algorithm_data.sugeno_0.up[i]
						  /model->algorithm_data.sugeno_0.down[i];
			else
				bad++;
			model->algorithm_data.sugeno_0.up[i]=0;
			model->algorithm_data.sugeno_0.down[i]=0;
		}
		break;
#endif

#ifdef USE_SUGENO_1
	case EC_SUGENO_1:
        for(i=0;i<model->num_output_vars;i++)
		{
			if(model->algorithm_data.sugeno_1.down[i]>ZERO_LIMIT)
				output[i]=model->algorithm_data.sugeno_1.up[i]
						  /model->algorithm_data.sugeno_1.down[i];
			else
				bad++;
			model->algorithm_data.sugeno_1.up[i]=0;
			model->algorithm_data.sugeno_1.down[i]=0;
		}
		break;
#endif

#ifdef USE_MAMDANI
	case EC_MAMDANI:
		for(i=0,idx=0;i<model->num_output_vars;i++)
		{
			a=0;
			b=0;
			start=0;

			// find first non-singleton distribution
			for(j=start;j<model->out_mf_num[i];j++,start++)
				if(model->out_mf[idx+j]._distribution!=exact) break;

			if(j<model->out_mf_num[i])
			{
				t=GetLevel(&model->out_mf[idx+j],0);	// start value \mu(0)
				n=min(model->algorithm_data.mamdani.level[idx+j],t);
			}

			for(x=0;x<1.0f;x+=0.01f)
			{	
				j=start;
				// calculate value of MF after accumulation
				c=n;		// move old "next" to current
				t=GetLevel(&model->out_mf[idx+j],x+0.01f);	// calcualte next
				n=min(model->algorithm_data.mamdani.level[idx+j],t);
				for(;j<model->out_mf_num[i];j++)
				{
					if(model->out_mf[idx+j]._distribution==exact) continue;

					t=min(model->algorithm_data.mamdani.level[idx+j],GetLevel(&model->out_mf[idx+j],x));
					if(t>c) c=t;
					t=min(model->algorithm_data.mamdani.level[idx+j],GetLevel(&model->out_mf[idx+j],x+0.001f));
					if(t>n) n=t;

					if(SupportMinus(&model->out_mf[idx+j])>x) break;
				}

				if(SupportPlus(&model->out_mf[idx+j])<x) 
				{			// move start to next non-singleton MF
					do	
						start++;
					while(model->out_mf[idx+start]._distribution==exact 
						&& start<model->out_mf_num[i]);
				}

				// calculate integrals
				a+=x*(c+n)*0.005f;
				b+=c*0.01f;

			}
				// process singletons
				for(j=0;j<model->out_mf_num[i];j++)
				{
					if(model->out_mf[idx+j]._distribution==exact)
					{
						a+=model->algorithm_data.mamdani.level[idx+j]
						   *model->out_mf[idx+j]._modalvalue;
						b+=model->algorithm_data.mamdani.level[idx+j];
					}
				}
			// }  /// moved above proces singletons!
			if(b>ZERO_LIMIT)
				output[i]=a/b;
			else
				bad++;
	
			idx+=model->out_mf_num[i];
		}

		for(i=0;i<MAX_OUTPUT_MF;i++)
			model->algorithm_data.mamdani.level[i]=0;

		break;
#endif

	default:
		bad=1;
	}


	return !bad;
}

size_t RuleSize(fuzzy_model *model)
{
	switch(model->inference_algorithm)
	{
	case EC_SUGENO_0: return sizeof(float);
	case EC_SUGENO_1: return sizeof(float)*(model->num_input_vars+1);
	case EC_MAMDANI: return sizeof(mamdani_type)*model->num_output_vars;
	default: return 1;
	}
}

void Init(fuzzy_model *model)
{
	counter i;

	if(model->inference_algorithm==EC_SUGENO_0)
		for(i=0;i<model->num_output_vars;i++) 
		{
			model->algorithm_data.sugeno_0.up[i]=0;
			model->algorithm_data.sugeno_0.down[i]=0;
		}
	if(model->inference_algorithm==EC_SUGENO_1)
		for(i=0;i<model->num_output_vars;i++) 
		{
			model->algorithm_data.sugeno_1.up[i]=0;
			model->algorithm_data.sugeno_1.down[i]=0;
		}
	if(model->inference_algorithm==EC_MAMDANI)
		for(i=0;i<MAX_OUTPUT_MF;i++) 
			model->algorithm_data.mamdani.level[i]=0;
}

float GetFloat(float f)
{
	char b[4];
	b[0]=((char*)&f)[3];
	b[1]=((char*)&f)[2];
	b[2]=((char*)&f)[1];
	b[3]=((char*)&f)[0];
	return *((float*)b);
}
