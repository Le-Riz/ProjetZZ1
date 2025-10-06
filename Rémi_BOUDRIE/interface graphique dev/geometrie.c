#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "geometrie.h"
#include "SDL.h"

line * createLine(SDL_Point p1, SDL_Point p2){
    line * L = calloc(1,sizeof(line));
    L->p1 = p1;
    L->p2 = p2;
    return L;
}

rectangle * createRectangle(SDL_Point p,int w, int h, int angle){
    rectangle * Rect = calloc(1,sizeof(rectangle));
    Rect->p1 = p;
    Rect->w = w;
    Rect->h = h;
    Rect->angle = angle;
    return Rect;
}

roundRectangle * createRoundRectangle(SDL_Point p,int w, int h, float radius){
    roundRectangle * RoundRect = calloc(1,sizeof(roundRectangle));
    RoundRect->p = p;
    RoundRect->w = w;
    RoundRect->h = h;
    RoundRect->radius = radius;
    return RoundRect;
}

circle * createCircle(SDL_Point p, float radius){
    circle * C = calloc(1,sizeof(circle));
    C->p = p;
    C->radius = radius;
    return C;
}

arc * createArc(SDL_Point p, float radius, float start, float end){
    arc * A = calloc(1,sizeof(arc));
    A->p = p;
    A->radius = radius;
    A->start = start;
    A->end = end;
    return A;
}

geometrie * createGeometrie(void * data, GeomType type, SDL_Color color, int thickness){
    geometrie * G = calloc(1,sizeof(geometrie));
    G->data = data;
    G->type = type;
    G->color = color;
    G->thick = thickness;
    return G;
}

SDL_Point changeRep(SDL_Point p, SDL_Rect rect,int width,int height){
    SDL_Point Out;
    Out.x = ((float)p.x)*((float)rect.w/(float)width);
    Out.y = ((float)p.y)*((float)rect.h/(float)height);
    return Out;
}

void drawline(Window * win, geometrie obj, SDL_Rect dstRect,int InWidth, int InHeight){
    line * L = (line *) obj.data;
    SDL_Point p1 = changeRep(L->p1, dstRect,InWidth,InHeight);
    SDL_Point p2 = changeRep(L->p2, dstRect,InWidth,InHeight);
    //printf("Line (%d,%d) (%d,%d)\n",p1.x,p1.y,p2.x,p2.y);
    SDL_Texture * saveText = SDL_GetRenderTarget(win->renderer);
    SDL_Texture * texture = SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,2,2);
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(win->renderer,texture);
    SDL_SetRenderDrawColor(win->renderer,obj.color.r,obj.color.g,obj.color.b,obj.color.a);
    SDL_Rect RectTmp = rectC(0,0,2,2);
    SDL_RenderDrawRect(win->renderer,&RectTmp);
    float decalage=0.;
    if(p1.y > p2.y){
        decalage = -360.;
    }
    float angle = atan2f((p2.y - p1.y),(p2.x - p1.x));
    SDL_Point C = pointC(0,(float)obj.thick/2.);
    SDL_SetRenderTarget(win->renderer,saveText);
    SDL_Rect Rect = rectC(p1.x,p1.y - (float)obj.thick/2.,sqrtf((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)),obj.thick);
    
    SDL_RenderCopyEx(win->renderer,texture,NULL,&Rect,decalage+toDeg(angle),&C,SDL_FLIP_NONE);
    //printf("Draw (%d,%d) (%d,%d)\n",p1.x,p1.y,p2.x,p2.y);
    SDL_DestroyTexture(texture);
}

void drawRect(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    rectangle * R = (rectangle *) obj.data;
    SDL_Point p1 = changeRep(R->p1,dstRect,InWidth,InHeight);
    SDL_Point p2 = changeRep(pointC(R->p1.x+cosf(toRad(R->angle))*(float)R->w,R->p1.y-sinf(toRad(R->angle))*(float)R->w),dstRect,InWidth,InHeight);
    SDL_Point p3 = changeRep(pointC(R->p1.x+cosf(toRad(90-R->angle))*(float)R->h,R->p1.y+sinf(toRad(90-R->angle))*(float)R->h),dstRect,InWidth,InHeight);
    SDL_Point Vect = pointC(p3.x - p1.x,p3.y - p1.y);
    SDL_Point p4 = pointC(p2.x+Vect.x,p2.y+Vect.y);
    line * l = calloc(1,sizeof(line));
    obj.data = l;
    obj.type = LINE;
    ((line *)obj.data)->p1 = p1;
    ((line *)obj.data)->p2 = p2;
    drawline(win,obj,dstRect,InWidth,InHeight);
    ((line *)obj.data)->p1 = p1;
    ((line *)obj.data)->p2 = p3;
    drawline(win,obj,dstRect,InWidth,InHeight);
    ((line *)obj.data)->p1 = p2;
    ((line *)obj.data)->p2 = p4;
    drawline(win,obj,dstRect,InWidth,InHeight);
    ((line *)obj.data)->p1 = p3;
    ((line *)obj.data)->p2 = p4;
    drawline(win,obj,dstRect,InWidth,InHeight);
    free(l);
}

void drawRectFill(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    rectangle * Rect = (rectangle *) obj.data;
    SDL_Point p1 = changeRep(Rect->p1, dstRect,InWidth,InHeight);
    SDL_Texture * saveText = SDL_GetRenderTarget(win->renderer);
    SDL_Texture * texture = SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,2,2);
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(win->renderer,texture);
    SDL_SetRenderDrawColor(win->renderer,obj.color.r,obj.color.g,obj.color.b,obj.color.a);
    SDL_Rect RectTmp = rectC(0,0,2,2);
    SDL_RenderDrawRect(win->renderer,&RectTmp);
    SDL_Point C = pointC(0,0);
    SDL_SetRenderTarget(win->renderer,saveText);
    SDL_Point changeDist = changeRep(pointC(Rect->p1.x + Rect->w,Rect->p1.y + Rect->h), dstRect,InWidth,InHeight);
    SDL_Rect R = rectC(p1.x,p1.y,changeDist.x-p1.x,changeDist.y-p1.y);

    SDL_RenderCopyEx(win->renderer,texture,NULL,&R,Rect->angle,&C,SDL_FLIP_NONE);
    //printf("Draw (%d,%d) (%d,%d)\n",p1.x,p1.y,p2.x,p2.y);
    SDL_DestroyTexture(texture);
}

void getPointPerDist(float x1, float y1, float x2, float y2, float dist, int invVect,Sint16 * resX, Sint16 * resY){
    SDL_FPoint p2 = {x1,y1};
    SDL_FPoint p1 = {x2,y2};
    SDL_FPoint VectOrt;
    if(invVect){
        VectOrt.x = -p2.y+p1.y;
        VectOrt.y = p2.x-p1.x;
    }else{
        VectOrt.x = p2.y-p1.y;
        VectOrt.y = -p2.x+p1.x;
    }
    
    float norm = sqrtf(VectOrt.x*VectOrt.x + VectOrt.y*VectOrt.y);
    SDL_FPoint VectNorm = {VectOrt.x/norm,VectOrt.y/norm};

    SDL_FPoint res = {p2.x + dist*VectNorm.x, p2.y + dist*VectNorm.y};
    *resX = res.x;
    *resY = res.y;
}

void fillCircleGap(Window * win, SDL_Color col, Sint16 * x,Sint16 * y,SDL_Rect dstRect, int InWidth, int InHeight){
    SDL_Point p;
    for(int i=0;i<3;i++){
        p = changeRep(pointC(x[i],y[i]),dstRect,InWidth,InHeight);
        x[i] = p.x;
        y[i] = p.y;
    }
    //filledPolygonRGBA(win->renderer,x,y,3,255,0,0,255);
    filledPolygonRGBA(win->renderer,x,y,3,col.r,col.g,col.b,col.a);
}

void drawArc(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    //SDL_Texture * TextSave = SDL_GetRenderTarget(win->renderer);
    //int dstRectSaveX = dstRect.x, dstRectSaveY = dstRect.y, saveAlpha = obj.color.a;
    //SDL_Texture * Texture = SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,dstRect.w,dstRect.h);
    //dstRect.x = 0;
    //dstRect.y = 0;
    obj.color.a = 255;
    //SDL_SetRenderTarget(win->renderer,Texture);
    //SDL_SetTextureBlendMode(Texture,SDL_BLENDMODE_BLEND);

    arc * Arc = (arc *)obj.data;
    int precision = (int)sqrtf((Arc->radius*Arc->radius)/10)+4;
    obj.type = LINE;
    line * Line= calloc(1,sizeof(line));
    obj.data = Line;
    line * remp = calloc(1,sizeof(line));
    Sint16 * Vx = calloc(3,sizeof(Sint16));
    Sint16 * Vy = calloc(3,sizeof(Sint16));
    if(precision > 0){
        #define PEN_DRAW_CIRCLE_MAX_PRECISION 500
        if(precision > PEN_DRAW_CIRCLE_MAX_PRECISION){
            precision = PEN_DRAW_CIRCLE_MAX_PRECISION;
        }
        int old_x = sinf(toRad(Arc->start+90))*Arc->radius;
        int old_y = cosf(toRad(Arc->start+90))*Arc->radius;
        float a = toRad(Arc->start);
        Vx[0] = Arc->p.x + old_x + sinf(toRad(Arc->start+90))*((float)obj.thick/2.);
        Vy[0] = Arc->p.y + old_y + cosf(toRad(Arc->start+90))*((float)obj.thick/2.);
    
        while(a < toRad(Arc->end)){
            if(a+toRad(360.0f/precision) < toRad(Arc->end)){
	            a += toRad(360.0f/precision);
            }else{
	            a = toRad(Arc->end);
            }
            int new_x = sinf(a+toRad(90))*Arc->radius;
            int new_y = cosf(a+toRad(90))*Arc->radius;
            ((line *)obj.data)->p1.x = Arc->p.x+old_x;
            ((line *)obj.data)->p1.y = Arc->p.y+old_y;
            ((line *)obj.data)->p2.x = Arc->p.x+new_x;
            ((line *)obj.data)->p2.y = Arc->p.y+new_y;
            getPointPerDist(((line *)obj.data)->p1.x,((line *)obj.data)->p1.y,((line *)obj.data)->p2.x,((line *)obj.data)->p2.y,obj.thick/2.,0,&Vx[2],&Vy[2]);
            drawline(win,obj,dstRect,InWidth,InHeight);
            Vx[1] = ((line *)obj.data)->p1.x;
            Vy[1] = ((line *)obj.data)->p1.y;
            SDL_FPoint Vect = {Vx[1]-Vx[0],Vy[1]-Vy[0]};
            Vx[1] = Vx[1] + Vect.x;
            Vy[1] = Vy[1] + Vect.y;
            fillCircleGap(win,obj.color,Vx,Vy,dstRect,InWidth,InHeight);
            obj.data = Line;

            getPointPerDist(((line *)obj.data)->p2.x,((line *)obj.data)->p2.y,((line *)obj.data)->p1.x,((line *)obj.data)->p1.y,obj.thick/2.,1,&Vx[0],&Vy[0]);
            old_x = new_x;
            old_y = new_y;
        }
        Vx[2] = Arc->p.x + sinf(toRad(Arc->end+90))*(Arc->radius+obj.thick/2.);
        Vy[2] = Arc->p.y + cosf(toRad(Arc->end+90))*(Arc->radius+obj.thick/2.);
        fillCircleGap(win,obj.color,Vx,Vy,dstRect,InWidth,InHeight);

        //SDL_SetRenderTarget(win->renderer,TextSave);
        //SDL_SetTextureAlphaMod(Texture,saveAlpha);
        //SDL_Rect R = rectC(dstRectSaveX,dstRectSaveY,dstRect.w,dstRect.h);
        //printf("Arc (%d,%d)\n",dstRectSaveX,dstRectSaveY);
        //SDL_RenderCopy(win->renderer,Texture,NULL,&R);
    }
    //SDL_DestroyTexture(Texture);
    free(Line);
    free(remp);
    free(Vx);
    free(Vy);
}

void drawCircleFill(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    SDL_Texture * TextSave = SDL_GetRenderTarget(win->renderer);
    int dstRectSaveX = dstRect.x, dstRectSaveY = dstRect.y, saveAlpha = obj.color.a;
    SDL_Texture * Texture = SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,dstRect.w,dstRect.h);
    dstRect.x = 0;
    dstRect.y = 0;
    obj.color.a = 255;
    SDL_SetRenderTarget(win->renderer,Texture);
    SDL_SetTextureBlendMode(Texture,SDL_BLENDMODE_BLEND);
    circle * Circle = (circle *)obj.data;
    int stepPrecision = 2;
    SDL_Point p = changeRep(Circle->p,dstRect,InWidth,InHeight);
    for(int rx = 0; rx <= Circle->radius; rx+=stepPrecision){
        int height = ((float)(Circle->radius))*sqrt(1-(1-rx/(float)Circle->radius)*(1-rx/(float)Circle->radius));
        SDL_FRect Prem = {p.x-Circle->radius+rx, p.y-height, stepPrecision, 2*height};
        SDL_FRect Deux = {p.x+Circle->radius-rx, p.y-height, stepPrecision, 2*height};
        SDL_SetRenderDrawColor(win->renderer,obj.color.r,obj.color.g,obj.color.b,obj.color.a);
        SDL_RenderFillRectF(win->renderer, &Prem);
        SDL_RenderFillRectF(win->renderer, &Deux);
    }

    SDL_SetRenderTarget(win->renderer,TextSave);
    SDL_SetTextureAlphaMod(Texture,saveAlpha);
    SDL_Rect R = rectC(dstRectSaveX,dstRectSaveY,dstRect.w,dstRect.h);
    SDL_RenderCopy(win->renderer,Texture,NULL,&R);
    SDL_DestroyTexture(Texture);
}

void drawCircle(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    circle * Circle = (circle *)obj.data;
    arc * Arc = calloc(1,sizeof(arc));
    Arc->p = Circle->p;
    Arc->radius = Circle->radius;
    Arc->start = 0;
    Arc->end = 360;
    obj.data = Arc;
    drawArc(win,obj,dstRect,InWidth,InHeight);
    free(Arc);
}

void drawRoundRect(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    roundRectangle * RdRect = (roundRectangle *)obj.data;
    arc * Arc = calloc(1,sizeof(arc));
    if(RdRect->radius <= (float)RdRect->h/2.){
        line * L = calloc(1,sizeof(line));
        geometrie geo = {L,LINE,obj.color,obj.thick};
        obj.type = ARC;
        obj.data = Arc;
        SDL_Rect RectRep = rectC(dstRect.x + RdRect->p.x*((float)dstRect.w/(float)InWidth),dstRect.y + RdRect->p.y*((float)dstRect.h/(float)InHeight),RdRect->w*((float)dstRect.w/(float)InWidth),RdRect->h*((float)dstRect.h/(float)InHeight));
        //printf("nouveau Rep %d,%d,%d,%d\n",RectRep.x,RectRep.y,RectRep.w,RectRep.h);
        float RectRadius = (RdRect->radius)*((float)dstRect.h/(float)InHeight);
        Arc->radius = RectRadius;
        dstRect.x = 0;
        dstRect.y = 0;
        dstRect.w = InWidth;
        dstRect.h = InHeight;
        for(int i=0;i<4;i++){
            L->p1.x = RectRep.x + (RectRadius)*((i+1)%2);
            L->p1.y = RectRep.y + RectRadius*(i%2);
            L->p2.x = L->p1.x + (RectRep.w - 2*RectRadius)*((i+1)%2);
            L->p2.y = L->p1.y + (RectRep.h - 2*RectRadius)*(i%2);
            Arc->p.x = RectRep.x + RectRadius;
            Arc->p.y = RectRep.y + RectRadius;
            Arc->start = 450 - i*90 %360;
            Arc->end = 540 - i*90 %360;
            if(i==1){
                L->p1.x += RectRep.w;
                L->p2.x += RectRep.w;
                Arc->p.x += RectRep.w - (2*RectRadius);
            }else if(i==2){
                L->p1.y += RectRep.h;
                L->p2.y += RectRep.h;
                Arc->p.x += RectRep.w - (2*RectRadius);
                Arc->p.y += RectRep.h - (2*RectRadius);
            }else if(i==3){
               Arc->p.y += RectRep.h - (2*RectRadius); 
            }
            drawline(win,geo,dstRect,InWidth,InHeight);
            //printf("line (%d,%d) (%d,%d), arc (%d,%d)\n",L->p1.x,L->p1.y,L->p2.x,L->p2.y,Arc->p.x,Arc->p.y);
            drawArc(win,obj,dstRect,InWidth,InHeight);
        }
        free(L);
    }else{
        printf("Erreur radius trop grand\n");
    }
    free(Arc);
}

void drawRoundRectFill(Window * win, geometrie obj, SDL_Rect dstRect, int InWidth, int InHeight){
    roundRectangle * RdRect = (roundRectangle *)obj.data;
    circle * Circle = calloc(1,sizeof(circle));
    if(RdRect->radius <= (float)RdRect->h/2.){
        SDL_Texture * TextSave = SDL_GetRenderTarget(win->renderer);
        int dstRectSaveX = dstRect.x, dstRectSaveY = dstRect.y, saveAlpha = obj.color.a, dstRectSaveW = dstRect.w, dstRectSaveH = dstRect.h;
        SDL_Texture * Texture = SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,dstRect.w,dstRect.h);
        dstRect.x = 0;
        dstRect.y = 0;
        obj.color.a = 255;
        SDL_SetRenderTarget(win->renderer,Texture);
        SDL_SetTextureBlendMode(Texture,SDL_BLENDMODE_BLEND);
        
        rectangle * Rect = calloc(1,sizeof(rectangle));
        Rect->angle = 0;
        geometrie geo = {Rect,RECTANGLEFILL,obj.color,obj.thick};
        obj.type = CIRCLEFILL;
        obj.data = Circle;
        SDL_Rect RectRep = rectC(RdRect->p.x*((float)dstRect.w/(float)InWidth),RdRect->p.y*((float)dstRect.h/(float)InHeight),RdRect->w*((float)dstRect.w/(float)InWidth),RdRect->h*((float)dstRect.h/(float)InHeight));
        float RectRadius = (RdRect->radius)*((float)dstRect.h/(float)InHeight);
        Circle->radius = (RdRect->radius)*((float)dstRect.h/(float)InHeight);
        dstRect.w = InWidth;
        dstRect.h = InHeight;
        //printf("Ori (%d,%d)\n",RectRep.x,RectRep.y);
        for(int i=0;i<4;i++){
            Rect->h = RectRep.h;
            Rect->w = RectRep.w;
            Circle->p.x = RectRep.x + RectRadius;
            Circle->p.y = RectRep.y + RectRadius;
            if(i==1){
                Rect->p1.x = RectRep.x;
                Rect->p1.y = RectRep.y + RectRadius;
                Rect->h -= RectRadius*2;
                //printf("i = 1 (%d,%d) %d %d\n",Rect->p1.x,Rect->p1.y,Rect->w,Rect->h);
                Circle->p.x += RectRep.w - (2*RectRadius);
                drawRectFill(win,geo,dstRect,InWidth,InHeight);
            }else if(i==2){
                Rect->p1.x = RectRep.x + RectRadius;
                Rect->p1.y = RectRep.y;
                Rect->w -= RectRadius*2;
                //printf("i = 1 (%d,%d) %d %d\n",Rect->p1.x,Rect->p1.y,Rect->w,Rect->h);
                Circle->p.x += RectRep.w - (2*RectRadius);
                Circle->p.y += RectRep.h - (2*RectRadius);
                drawRectFill(win,geo,dstRect,InWidth,InHeight);
            }else if(i==3){
               Circle->p.y += RectRep.h - (2*RectRadius); 
            }
            
            drawCircleFill(win,obj,dstRect,InWidth,InHeight);
        }

        SDL_SetRenderTarget(win->renderer,TextSave);
        SDL_SetTextureAlphaMod(Texture,saveAlpha);
        SDL_Rect R = rectC(dstRectSaveX,dstRectSaveY,dstRectSaveW,dstRectSaveH);
        SDL_RenderCopy(win->renderer,Texture,NULL,&R);
        //SDL_RenderPresent(win->renderer);
        SDL_DestroyTexture(Texture);
    }else{
        printf("Erreur radius trop grand\n");
    }
    free(Circle);
}

void drawGeom(Window * win, geometrie geo, SDL_Rect dstRect,int InWidth, int InHeight){
    switch (geo.type){
    case LINE:
        drawline(win,geo,dstRect,InWidth,InHeight);
        break;

    case RECTANGLE:
        drawRect(win,geo,dstRect,InWidth,InHeight);
        break;

    case ARC:
        drawArc(win,geo,dstRect,InWidth,InHeight);
        break;

    case CIRCLEFILL:
        drawCircleFill(win,geo,dstRect,InWidth,InHeight);
        break;

    case CIRCLE:
        drawCircle(win,geo,dstRect,InWidth,InHeight);
        break;

    case ROUNDRECTANGLE:
        drawRoundRect(win,geo,dstRect,InWidth,InHeight);
        break;

    case ROUNDRECTANGLEFILL:
        drawRoundRectFill(win,geo,dstRect,InWidth,InHeight);
        break;

    default:
        break;
    }
}
