//
//  Coloring.frag
//
//
//  Created by renhong on 7/24/14.
//
//

const char* Coloring_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec4 v_start_color;//start color
uniform vec4 v_end_color;//end color
uniform vec4 v_touched_color;
uniform int i_color_model;//coloring model
uniform sampler2D s_texture_shape;//draw target shape(shape by dif color)
uniform sampler2D s_texture_shape_bounds;//draw target bound box
uniform vec2 v_texture_shape_size;
uniform int i_need_coloring_black_target;//need draw the black area?

const int kColorModelPlat = 0;
const int kColorModelSmoothH = 1;
const int kColorModelSmoothV = 2;
const int kColorModelSmoothP = 3;
const vec3 kBlackColor = vec3(0.0, 0.0, 0.0);
const vec4 kBlackColorRGBA = vec4(0.0, 0.0, 0.0, 1.0);

vec2 pointCross(const vec2 begin, const vec2 end, const vec2 pt);
bool vec3equal(vec3 pLeft, vec3 pRight);
vec4 colorWithModel(vec4 pStartColor, vec4 pEndColor, int pColoringModel, vec4 pShapeBounds, vec2 pCoord);

void main(){
    if((i_need_coloring_black_target == 0) && vec3equal(v_touched_color.rgb, kBlackColor.rgb)){//ignore the fragement in black line frame if we don't need to draw it. eg. the black line frame.
        discard;
    }
    vec2 lReallyTexCoord = v_texCoord;
    vec4 lColorInTemplate = texture2D(s_texture_shape, lReallyTexCoord);
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);return;
    if(vec3equal(lColorInTemplate.rgb, v_touched_color.rgb)){//fragement in the shape
        if(kColorModelPlat == i_color_model || v_start_color == v_end_color){
            gl_FragColor = v_start_color;
        }
        else {
            vec2 texColorCoord = lColorInTemplate.rg;
            vec4 shape_bounds = texture2D(s_texture_shape_bounds, texColorCoord);
            
            gl_FragColor = colorWithModel(v_start_color, v_end_color, i_color_model, shape_bounds, lReallyTexCoord);
        }
    }
    else{
        discard;
    }
}

// 二维空间点到直线的垂足
vec2 pointCross(const vec2 begin, const vec2 end, const vec2 pt){
    vec2 retVal;
    
    float dx = begin.x - end.x;
    float dy = begin.y - end.y;
    if(abs(dx) < 0.00000001 && abs(dy) < 0.00000001 ){
        retVal = begin;
        return retVal;
    }
    
    float u = (pt.x - begin.x)*(begin.x - end.x) + (pt.y - begin.y)*(begin.y - end.y);
    u = u/((dx*dx)+(dy*dy));
    
    retVal.x = begin.x + u*dx;
    retVal.y = begin.y + u*dy;
    
    return retVal;
}

bool vec3equal(vec3 pLeft, vec3 pRight){
    float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
    //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
    vec3 lTmp = pLeft - pRight;
    float lTmpMax = max(max(abs(lTmp.r), abs(lTmp.g)), abs(lTmp.b));
    if(lTmpMax <= EPSINON){
        return true;
    }
    return false;
}

bool vec4equal(vec4 pLeft, vec4 pRight){
    float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
    //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
    vec4 lTmp = pLeft - pRight;
    float lTmpMax1 = max(abs(lTmp.r), abs(lTmp.g));
    float lTmpMax2 = max(abs(lTmp.b), abs(lTmp.a));
    float lTmpMax = max(lTmpMax1, lTmpMax2);
    if(lTmpMax <= EPSINON){
        return true;
    }
    return false;
}

vec4 colorWithModel(vec4 pStartColor, vec4 pEndColor, int pColoringModel, vec4 pShapeBounds, vec2 pCoord){
    vec4 texColorBrush = pStartColor;
    if(kColorModelPlat == pColoringModel){
        //return texColorBrush;
    }
    else if(pStartColor == pEndColor){
        //return texColorBrush;
    }
    else{
        vec3 deltaColor = (pEndColor - pStartColor).rgb;
        if(kColorModelSmoothH == pColoringModel){//H-X
            float rateX = (pCoord.x - pShapeBounds.r) / (pShapeBounds.g - pShapeBounds.r);
            texColorBrush = vec4(pStartColor.rgb + deltaColor * rateX, 1.0);
        }
        else if(kColorModelSmoothV == pColoringModel){//V-Y
            float rateY = (pCoord.y - pShapeBounds.b) / (pShapeBounds.a - pShapeBounds.b);
            texColorBrush = vec4(pStartColor.rgb + deltaColor * rateY, 1.0);
        }
        else if(kColorModelSmoothP == pColoringModel){//P-XY
            vec2 lefttop = vec2(pShapeBounds.r * v_texture_shape_size.x, pShapeBounds.b * v_texture_shape_size.y);
            vec2 rightbottom = vec2(pShapeBounds.g * v_texture_shape_size.x, pShapeBounds.a * v_texture_shape_size.y);
            vec2 cur = vec2(pCoord.x * v_texture_shape_size.x, (pCoord.y) * v_texture_shape_size.y);
            vec2 ptCross = pointCross(lefttop, rightbottom, cur);
            float width = (pShapeBounds.g - pShapeBounds.r) * v_texture_shape_size.x;
            float height = (pShapeBounds.a - pShapeBounds.b) * v_texture_shape_size.y;
            
            ptCross = vec2(ptCross.x / v_texture_shape_size.x, ptCross.y / v_texture_shape_size.y);
            
            if(height <= width){//横向矩形或正方形
                texColorBrush = vec4(texColorBrush.rgb + (ptCross.x - pShapeBounds.r) / (pShapeBounds.g - pShapeBounds.r) * deltaColor, 1.0);
            }
            else if(height > width){//纵向矩形或
                texColorBrush = vec4(texColorBrush.rgb + (ptCross.y - pShapeBounds.b) / (pShapeBounds.a - pShapeBounds.b) * deltaColor, 1.0);
            }
        }
    }
    return texColorBrush;
}
);