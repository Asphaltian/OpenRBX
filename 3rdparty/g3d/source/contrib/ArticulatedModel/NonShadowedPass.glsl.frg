/**
  @file SuperShader.glsl.frg
  @author Morgan McGuire matrix@graphics3d.com

  For use with G3D::SuperShader.

 */

/** World space, normalized (TODO: non-directional light, multiple lights) */
uniform vec4        lightPosition;
uniform vec3        lightColor;

uniform vec3        environmentConstant;
uniform samplerCube environmentMap;

// Material
#ifdef REFLECTCONSTANT
    uniform vec3        reflectConstant;
#endif

#ifdef REFLECTMAP
    uniform sampler2D   reflectMap;
#endif

#ifdef SPECULARCONSTANT
    uniform vec3        specularConstant;
#endif

#ifdef SPECULARMAP
    uniform sampler2D   specularMap;
#endif

#ifdef SPECULAREXPONENTCONSTANT
    uniform vec3        specularExponentConstant;
#endif

#ifdef SPECULAREXPONENTMAP
    uniform sampler2D   specularExponentMap;
#endif

#ifdef EMITCONSTANT
    uniform vec3        emitConstant;
#endif

#ifdef EMITMAP
    uniform sampler2D   emitMap;
#endif

#ifdef DIFFUSECONSTANT
    uniform vec3        diffuseConstant;
#endif

#ifdef DIFFUSEMAP
    uniform sampler2D   diffuseMap;
#endif

#ifdef NORMALBUMPMAP
    /** Multiplier for bump map.  Typically on the range [0, 0.05]
      This increases with texture scale and bump height. */
    uniform float       bumpMapScale;

    /** xyz = normal, w = bump height */
    uniform sampler2D   normalBumpMap;

    /** Un-normalized (interpolated) tangent space eye vector */
    varying vec3        _tsE;
	varying vec4        tan_X, tan_Y, tan_W;
#endif

varying vec4        tan_Z;
uniform vec3        ambientTop;
uniform vec3        ambientBottom;

// World parameters
varying vec3        wsPosition;
varying vec2        texCoord;

/** Set to -1 to flip the normal. */
uniform float       backside;

/** Used for "ambient occlusion" */
varying float       accessibility;

void main(void) {

    vec3 wsEyePos = g3d_CameraToWorldMatrix[3].xyz;

#   ifdef NORMALBUMPMAP
        // Convert bumps to a world space distance
        float  bump   = (texture2D(normalBumpMap, texCoord).w - 0.5) * bumpMapScale;

	    vec3 tsE = normalize(_tsE);

        // Offset the texture coord.  Note that texture coordinates are inverted (in the y direction)
	    // from TBN space, so we must flip the y-axis.

        vec2 offsetTexCoord = texCoord.xy + vec2(tsE.x, -tsE.y) * bump;

	    // note that the columns might be slightly not orthogonal due to interpolation
	    mat4 tangentToWorld = mat4(tan_X, tan_Y, tan_Z, tan_W);
	    
        // Take the normal map values back to (-1, 1) range to compute a tangent space normal
        vec3 tsN = ((texture2D(normalBumpMap, offsetTexCoord).xyz - vec3(0.5, 0.5, 0.5)) * 2.0);

	    // Take the normal to world space 
	    vec3 wsN = (tangentToWorld * vec4(tsN, 0.0)).xyz * backside;

#   else

        // World space normal
        vec3 wsN = tan_Z.xyz * backside;

        vec2 offsetTexCoord = texCoord;
#   endif


    // Light vector      
	vec3 wsL = normalize(lightPosition.xyz - wsPosition.xyz * lightPosition.w);


#   if defined(REFLECTCONSTANT) || defined(REFLECTMAP) || defined(SPECULARCONSTANT) || defined(SPECULARMAP)
        // Eye vector
        vec3 wsE = wsEyePos - wsPosition;
	    // or... (tangentToWorld * vec4(tsE, 0.0)).xyz;

        // Reflection vector
        vec3 wsR = normalize((wsN * 2.0 * dot(wsN, wsE)) - wsE);
#   endif

#   if (defined(DIFFUSECONSTANT) || defined(DIFFUSEMAP))
        vec4 diffuseColor =
#       ifdef DIFFUSECONSTANT
            vec4(diffuseConstant, 1)
#           ifdef DIFFUSEMAP
                * texture2D(diffuseMap, offsetTexCoord)
#           endif
#       else
            texture2D(diffuseMap, offsetTexCoord)
#       endif
        ;
#   endif


#   if defined(EMITCONSTANT) || defined(EMITMAP)     
        vec4 emitColor =
#       ifdef EMITCONSTANT
            vec4(emitConstant, 1)
#           ifdef EMITMAP
                * texture2D(emitMap, offsetTexCoord)
#          endif
#       else
            texture2D(emitMap, offsetTexCoord)
#       endif
        ;
#   endif


#   if defined(SPECULARCONSTANT) || defined(SPECULARMAP)     
        vec4 specularColor =
#       ifdef SPECULARCONSTANT
            vec4(specularConstant, 1)
#           ifdef SPECULARMAP
                * texture2D(specularMap, offsetTexCoord)
#          endif
#       else
            texture2D(specularMap, offsetTexCoord)
#       endif
        ;
#   endif


#   if defined(SPECULAREXPONENTCONSTANT) || defined(SPECULAREXPONENTMAP)     
        vec4 specularExponentColor =
#       ifdef SPECULAREXPONENTCONSTANT
            vec4(specularExponentConstant, 1.0)
#           ifdef SPECULAREXPONENTMAP
                * texture2D(specularExponentMap, offsetTexCoord)
#           endif
#       else
            texture2D(specularExponentMap, offsetTexCoord)
#       endif
        ;
#   else
        vec4 specularExponentColor = vec4(1.0, 1.0, 1.0, 1.0);
#   endif


#   if defined(REFLECTCONSTANT) || defined(REFLECTMAP)     
        vec4 reflectColor =
#       ifdef REFLECTCONSTANT
            vec4(reflectConstant, 1)
#           ifdef REFLECTMAP
                 * texture2D(reflectMap, offsetTexCoord)
#           endif
#       else
            texture2D(reflectMap, offsetTexCoord)
#       endif
        ;
#   endif

    vec3 ambient = ambientTop + (ambientTop - ambientBottom) * min(wsN.y, 0.0);

    gl_FragColor.rgb =
        vec3(0.0, 0.0, 0.0)
#       if defined(EMITCONSTANT) || defined(EMITMAP)
            + emitColor.rgb
#       endif

        + accessibility * (

#       if defined(DIFFUSECONSTANT) || defined(DIFFUSEMAP)
           + diffuseColor.rgb * 
            (ambient.rgb + max(dot(wsL, wsN), 0.0) * lightColor.rgb)
#       endif

#       if defined(SPECULARCONSTANT) || defined(SPECULARMAP)
            + pow(vec3(max(dot(wsL, wsR), 0.0)), specularExponentColor.rgb) * lightColor.rgb * specularColor.rgb
#       endif

#       if defined(REFLECTCONSTANT) || defined(REFLECTMAP)     
            + textureCube(environmentMap, wsR).rgb * environmentConstant.rgb
            * reflectColor.rgb
#       endif
        );

    gl_FragColor.a = 1.0
#       if defined(EMITCONSTANT) || defined(EMITMAP)
            * emitColor.a
#       endif

#       if defined(DIFFUSECONSTANT) || defined(DIFFUSEMAP)
           * diffuseColor.a
#       endif

#       if defined(SPECULARCONSTANT) || defined(SPECULARMAP)
            * specularColor.a
#       endif

#       if defined(SPECULAEXPONENTRCONSTANT) || defined(SPECULAREXPONENTMAP)
            * specularExponentColor.a
#       endif

#       if defined(REFLECTCONSTANT) || defined(REFLECTMAP)     
            * reflectColor.a
#       endif
        ;
}
