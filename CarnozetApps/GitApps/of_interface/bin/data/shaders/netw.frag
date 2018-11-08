#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
//#define GL_FRAGMENT_PRECISION_HIGH 1





uniform sampler2DRect tex0;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position texture

uniform int resolution;
uniform vec2  screen;



// forces

uniform float k;
uniform float l0;
uniform float l0max;
uniform float z;
uniform int size;




void main(void){
    vec2 st=gl_TexCoord[0].xy;
    vec3 vel = texture2DRect(tex0, st).xyz;
    vec3 pos = texture2DRect( posData, st).xyz;
    float l0norm = l0/resolution;
    vec3 distbuf;
    float normbuf;
    vec3 linbuf;
    float leftNormBuf;
    float l0maxnorm;
    if(l0max==0){
        l0maxnorm = 9999;
    }
    else{
        l0maxnorm = l0max/resolution;
    }

    for(int i = 1 ; i<=size;i++){



        leftNormBuf = -1.0;
        //        linbuf= vec3(l0max+1);

        if(st.x<resolution-i){
            vec3 poso = texture2DRect( posData , st+vec2(i,0)).xyz;
            distbuf=pos-poso;
            linbuf=distbuf;
            normbuf = length(distbuf);
            leftNormBuf = normbuf;
            if(normbuf>0 && normbuf<l0maxnorm){
                vel-=normalize(distbuf)*(normbuf-i*l0norm)*k;
            }
        }
        if(st.x>i){
            vec3 opos = texture2DRect( posData , st+vec2(-i,0)).xyz;
            distbuf=pos-opos;
            normbuf = length(distbuf);

            if(normbuf>0 && normbuf<l0maxnorm){
                vel-=normalize(distbuf)*(normbuf-i*l0norm)*k;
                //            if(st.y<resolution-1&&length(distbuf)<netmax){
                //                vel-=z*(distbuf+linbuf);
                //            }
                if(leftNormBuf<l0maxnorm && leftNormBuf>=0){
                    vel-=z*(distbuf+linbuf);
                }
            }




        }

    }

    
    
    gl_FragColor.xyz = vel;
    
}
