/* Execute selected original i386 driver routines, with explicitly listed host hooks.
 * Build with a 32-bit C compiler. The wrapper verifies the complete DLL SHA-256.
 * This is an isolated routine probe, not an end-to-end RIPtel renderer.
 */
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void require(int ok,const char *label);
static unsigned char *image;
static unsigned char context[256], table[64], style[0x61], viewport[128];
static char temporary[8192];
static unsigned char vector_state[0x2300], graphics_state[64], port[128];
struct render_args { uint32_t value[26]; };
static struct render_args captured;
static unsigned render_calls;
static void capture_render(struct render_args args) { captured=args; ++render_calls; }
static unsigned font_load_calls, font_lookup_missing;
static unsigned loaded_size, loaded_face;
static const char *renderer_message;
static unsigned font_lookup(void *ctx,const char *name) {
    (void)ctx;(void)name;return font_lookup_missing?0xffffffffu:1;
}
static void renderer_log(const char *message) { renderer_message=message; }
static unsigned font_load(void *ctx,unsigned index,unsigned size,unsigned face) {
    (void)ctx;require(index==1,"resolved font index reaches loader");
    ++font_load_calls;loaded_size=size&0xffff;loaded_face=face&0xffff;return 0;
}
static unsigned __attribute__((stdcall)) gdi_attribute(unsigned dc,unsigned value) { (void)dc;(void)value;return 0; }
static unsigned __attribute__((stdcall)) client_rect(unsigned hwnd,int32_t *rect) {
    (void)hwnd;rect[0]=rect[1]=0;rect[2]=640;rect[3]=480;return 1;
}

static unsigned protection, checks;
static const char *error_message;
static uint16_t u16(const void *p) { uint16_t v; memcpy(&v,p,2); return v; }
static uint32_t u32(const void *p) { uint32_t v; memcpy(&v,p,4); return v; }
static void put16(void *p,uint16_t v) { memcpy(p,&v,2); }
static void put32(void *p,uint32_t v) { memcpy(p,&v,4); }
static void pointer(void *p,void *v) { put32(p,(uint32_t)(uintptr_t)v); }
static void fail(const char *s) { fprintf(stderr,"%s: %s\n",s,strerror(errno)); exit(1); }
static void require(int ok,const char *label) {
    ++checks;
    if (!ok) { fprintf(stderr,"FAIL: %s\n",label); exit(1); }
}
static void noop(void) {}
static unsigned protected_style(void *ctx,int slot) { (void)ctx; (void)slot; return protection; }
static void report_error(void *ctx,unsigned level,const char *name,const char *message) {
    (void)ctx; (void)level; (void)name; error_message=message;
}
static void *scratch(void *ctx) { (void)ctx; return temporary; }
static void hook(uint32_t rva,void (*fn)(void)) {
    image[rva]=0xe9;
    put32(image+rva+1,(uint32_t)(uintptr_t)fn-(uint32_t)(uintptr_t)(image+rva+5));
}
static void load(const char *path) {
    FILE *f=fopen(path,"rb"); if(!f) fail("open DLL");
    if(fseek(f,0,SEEK_END)) fail("seek DLL");
    long size=ftell(f); rewind(f);
    require(size==592896,"DLL size");
    unsigned char *raw=malloc((size_t)size); if(!raw) fail("allocate file");
    require(fread(raw,1,(size_t)size,f)==(size_t)size,"read DLL"); fclose(f);
    uint32_t pe=u32(raw+0x3c), opt=pe+24;
    require(u16(raw+pe+4)==0x14c && u32(raw+opt+28)==0x10000000,"PE32 image identity");
    int flags=MAP_PRIVATE|MAP_ANON;
#ifdef __FreeBSD__
    flags |= MAP_FIXED|MAP_EXCL;
#elif defined(MAP_FIXED_NOREPLACE)
    flags |= MAP_FIXED_NOREPLACE;
#endif
    image=mmap((void *)0x10000000,0xa1000,PROT_READ|PROT_WRITE|PROT_EXEC,flags,-1,0);
    if(image==MAP_FAILED) fail("map driver");
    require(image==(void *)0x10000000,"preferred image base available");
    for(unsigned i=0;i<u16(raw+pe+6);++i) {
        unsigned char *s=raw+opt+u16(raw+pe+20)+40*i;
        uint32_t rva=u32(s+12), length=u32(s+16), offset=u32(s+20);
        require((uint64_t)offset+length<=(unsigned long)size && (uint64_t)rva+length<=0xa1000,"section bounds");
        memcpy(image+rva,raw+offset,length);
    }
    free(raw);
    /* UI/context synchronization, style protection, error sink, scratch allocation,
     * macro expansion and scratch release. The tested numeric handler is untouched.
     */
    hook(0x45038,noop);
    hook(0x3935b,(void (*)(void))protected_style);
    hook(0x38e47,(void (*)(void))report_error);
    hook(0x15396,(void (*)(void))scratch);
    hook(0x153fe,noop);
    hook(0x3988f,noop);
    hook(0x398ee,noop);
    hook(0x4b0e4,noop);
}
static void prepare(void) {
    memset(context,0,sizeof context); memset(table,0,sizeof table);
    memset(style,0xa5,sizeof style); memset(viewport,0,sizeof viewport);
    pointer(context+0xa,table); pointer(table,style);
    pointer(context+0xe,viewport);
    put32(viewport+0x24,1280); put32(viewport+0x28,960);
    put32(viewport+0x2c,640); put32(viewport+0x30,480);
    protection=0; error_message=NULL;
}
static void apply(uint32_t *args,const char *name) {
    typedef void (*handler)(void *,unsigned,uint32_t *,const char *);
    ((handler)(image+0x1adc0))(context,11,args,name);
}
static void decode(const char *wire,uint32_t *args) {
    static const unsigned widths[]={1,1,4,2,2,2,2,2,2,2,6};
    typedef uint32_t (*decoder)(const char *);
    for(unsigned i=0,pos=0;i<11;++i) {
        char value[8]; memcpy(value,wire+pos,widths[i]); value[widths[i]]=0;
        args[i]=((decoder)(image+0x211d6))(value); pos+=widths[i];
    }
}
int main(int argc,char **argv) {
    if(argc!=2) { fprintf(stderr,"Usage: probe DLL\n"); return 2; }
    load(argv[1]);
    uint32_t args[11]; decode("0000BW1Q080000001a1a000000",args);
    const uint32_t expected[]={0,0,736,90,8,0,0,0,100,100,0};
    require(!memcmp(args,expected,sizeof args),"original decoder: requested example");
    prepare(); apply(args,"Marin");
    require(!error_message && style[0x24]==1 && !strcmp((char *)style+0x27,"Marin"),"example installs extended style");
    require(u32(style+0x53)==736 && u16(style+0x4f)==90 && u16(style+0x57)==8 && u16(style+0x5f)==100,"example stored field values");
    unsigned char baseline[sizeof style]; memcpy(baseline,style,sizeof style);
    args[9]=4095;args[10]=0xffffffff;
    prepare();apply(args,"Marin");
    require(!error_message && !memcmp(style,baseline,sizeof style),"WW and six reserved characters have no handler effect");
    decode("0000BW1Q080000001a1a000000",args);
    for(unsigned field=5;field<=6;++field) {
        for(unsigned value=0;value<4096;++value) {
            args[field]=value;prepare();apply(args,"Marin");
            int valid=value==0 || value==900 || value==1800 || value==2700;
            require((error_message==NULL)==valid,"exhaustive rotation domain");
            if(!valid) require(style[0x24]==0xa5,"rejected rotation leaves style intact");
        }
        args[field]=0;
    }
    args[8]=0;prepare();apply(args,"Marin");
    require(error_message && !strcmp(error_message,"Character spacing percentage cannot be zero!"),"zero spacing diagnostic");
    args[8]=100;prepare();apply(args,"");
    require(error_message && !strcmp(error_message,"No typeface name specified!"),"empty typeface diagnostic");
    prepare();protection=1;apply(args,"Marin");
    require(error_message && style[0x24]==0xa5,"protected style rejected before mutation");
    for(unsigned value=0;value<64;++value) {
        args[0]=value;args[1]=63-value;prepare();apply(args,"Marin");
        require(!error_message && style[0x51]==value && style[0x52]==63-value,"all single-digit values stored without rejection");
    }
    args[0]=args[1]=args[2]=0;
    for(unsigned value=0;value<4096;++value) {
        args[3]=value;args[4]=value;args[7]=value;args[8]=value?value:1;
        prepare();apply(args,"Marin");
        require(!error_message && u16(style+0x4f)==value && u16(style+0x57)==value && u16(style+0x5d)==value && u16(style+0x5f)==args[8],"size shadow baseline spacing full wire domains");
    }
    args[3]=25;args[4]=1;args[7]=0;args[8]=100;
    for(unsigned value=0;value<2048;++value) {
        args[2]=value;prepare();apply(args,"Marin");
        require(!error_message && u32(style+0x53)==value,"low eleven flag bits accepted");
    }
    args[2]=0xffffff;prepare();apply(args,"Marin");
    require(!error_message && u32(style+0x53)==0xffffff,"maximum flag value accepted");
    /* The radix predicate reads only this synthetic parser state and the original
     * dispatch record. No parser or decoder code is hooked.
     */
    typedef unsigned (*predicate)(void *);
    unsigned char parser[256]={0};pointer(parser+0x30,image+0x80820+75*40);
    parser[0x38]=36;
    require(((predicate)(image+0x39d70))(parser)==0,"y selects base64 with global base36");
    parser[0x38]=64;
    require(((predicate)(image+0x39d70))(parser)==0,"y selects base64 with global base64");
    /* Exercise the original ordinary graphics-text consumer. Intercept only its
     * final 26-argument renderer call, after its original state interpretation.
     */
    unsigned char saved_renderer[5];memcpy(saved_renderer,image+0x40ed0,5);
    hook(0x40ed0,(void (*)(void))capture_render);
    typedef void (*placer)(void *,unsigned,unsigned,const char *);
    for(unsigned sr=0;sr<4;++sr) for(unsigned cr=0;cr<4;++cr) {
        args[2]=0;args[3]=25;args[4]=0;args[5]=sr*900;args[6]=cr*900;args[7]=0;args[8]=100;
        prepare();apply(args,"Marin");render_calls=0;
        pointer(viewport,viewport+64);put16(viewport+65,1280);put16(viewport+67,960);
        pointer(context+0x4e,vector_state);
        ((placer)(image+0x42870))(context,200,100,"AB");
        const unsigned sdir[]={0,3,2,1};
        require(render_calls==1 && (captured.value[16]&0xffff)==sdir[sr] && (captured.value[17]&0xffff)==cr,"all 16 rotation pairs reach renderer");
    }
    for(unsigned flags=0;flags<2048;++flags) {
        args[2]=flags;args[4]=8;args[5]=args[6]=0;args[7]=20;args[8]=125;
        prepare();apply(args,"Marin");render_calls=0;
        pointer(viewport,viewport+64);put16(viewport+65,1280);put16(viewport+67,960);
        pointer(context+0x4e,vector_state);
        ((placer)(image+0x42870))(context,200,100,"AB");
        const uint32_t *a=captured.value;
        require(render_calls==1 && (a[11]&0xffff)==(flags&3) && (a[12]&0xffff)==(flags&12),"face and decoration flags reach renderer");
        require((a[14]&0xffff)==((flags>>6)&3)+1 && (a[15]&0xffff)==8 && (a[21]&0xffff)==((flags>>4)&3),"shadow mode distance and horizontal justification reach renderer");
        int baseline=(flags&512)?10:20;if(flags&256)baseline=-baseline;
        require((int16_t)a[19]==baseline && (int16_t)a[20]==((flags&1024)?-125:125),"baseline conversion sign and spacing sign reach renderer");
        require((a[18]&0xffff)==0,"y has no vertical-alignment input to renderer");
    }
    memcpy(image+0x40ed0,saved_renderer,5);
    hook(0x3fb50,(void (*)(void))font_lookup);
    hook(0x3fdf0,(void (*)(void))renderer_log);
    hook(0x4f230,noop);
    hook(0x40170,(void (*)(void))font_load);
    pointer(image+0x96400,(void *)gdi_attribute);
    pointer(image+0x963f0,(void *)gdi_attribute);
    pointer(image+0x963f8,(void *)gdi_attribute);
    pointer(image+0x96644,(void *)client_rect);
    prepare();memset(vector_state,0,sizeof vector_state);memset(port,0,sizeof port);
    pointer(context+0x4e,vector_state);pointer(context+0x3a,graphics_state);pointer(context+0x22,port);
    put16(vector_state+6,1280);put16(vector_state+8,960);put32(port+0x3a,640);
    typedef void (*renderer)(struct render_args);
    captured.value[0]=(uint32_t)(uintptr_t)context;
    captured.value[10]=(uint32_t)(uintptr_t)"Marin";
    captured.value[13]=90;captured.value[14]=4;captured.value[15]=8;captured.value[17]=0;
    captured.value[11]=3;captured.value[20]=100;captured.value[25]=0;
    font_lookup_missing=1;font_load_calls=0;
    ((renderer)(image+0x40ed0))(captured);
    require(font_load_calls==0,"unknown typeface causes no fallback font load");
    font_lookup_missing=0;captured.value[13]=0;error_message=NULL;
    ((renderer)(image+0x40ed0))(captured);
    require(error_message && !strcmp(error_message,"Font size cannot be 0"),"zero font size is deferred renderer error");
    captured.value[13]=90;renderer_message=NULL;
    ((renderer)(image+0x40ed0))(captured);
    require(font_load_calls==1 && loaded_size==900 && loaded_face==3,"font size multiplied by ten and bold italic passed to loader");
    require(renderer_message && !strcmp(renderer_message,"Failed to load vector font"),"font load failure diagnostic");
    require(u16(vector_state+0x6e)==36 && u16(vector_state+0x70)==36,"font DPI derived from client dimensions and world frame");
    require(u16(vector_state+0x1d24)==4,"shadow distance scales by canvas width over world width");
    require(u16(vector_state+0x1d22)==4,"shadow direction retained");
    printf("PASS: %u checks against original decoder, radix selector, handler and text consumer\n",checks);
    return 0;
}
