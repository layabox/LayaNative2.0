namespace laya
{
    struct CMD_ii
    {
        int i;
        int j;
    };
    struct CMD_if
    {
        int i;
        float j;
    };
    struct CMD_iff
    {
        int i;
        float x;
        float y;
    };
    struct CMD_ifff
    {
        int i;
        float x;
        float y;
        float z;
    };
    struct CMD_iffff
    {
        int i;
        float x;
        float y;
        float w;
        float h;
    };
    struct CMD_ifffffffff
    {
        int i;
        float x;
        float y;
        float w;
        float h;
        float x1;
        float y1;
        float w1;
        float h1;
    };
    struct CMD_iif
    {
        int i;
        int j;
        float x;
    };
    struct CMD_iiff
    {
        int i;
        int j;
        float x;
        float y;
    };
    struct CMD_iifff
    {
        int i;
        int j;
        float x;
        float y;
        float z;
    };
    struct CMD_iiffff
    {
        int i;
        int j;
        float x;
        float y;
        float w;
        float h;
    };
    struct CMD_iii
    {
        int i;
        int j;
        int k;
    };
    struct CMD_iiifii
    {
        int i;
        int j;
        int k;
        float l;
        int m;
        int n;
    };
    struct CMD_iiii
    {
        int i;
        int j;
        int k;
        int l;
    };
    struct CMD_iiiii
    {
        int i;
        int j;
        int k;
        int l;
        int m;
    };
    struct CMD_iiiiii
    {
        int i;
        int j;
        int k;
        int l;
        int m;
        int n;
    };
    struct CMD_iiiiiii
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
    };
    struct CMD_iiiiiiii
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
        int h;
    };
    struct CMD_iiiiiiiii
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
        int h;
        int k;
    };
    struct CMD_iiiiiiiiiii
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
        int h;
        int i;
        int j;
        int k;
    };
    struct CMD_fi
    {
        float x;
        int y;
    };
    struct CMD_ff
    {
        float x;
        float y;
    };
    struct CMD_ffi
    {
        float x;
        float y;
        int i;
    };
    struct CMD_ifi
    {
        int a;
        float b;
        int c;
    };
    struct CMD_ffii
    {
        float x;
        float y;
        int i;
        int j;
    };
    struct CMD_fff
    {
        float x;
        float y;
        float z;
    };
    struct CMD_fffiiii
    {
        float x;
        float y;
        float w;
        int   h;
        int   i;
        int   j;
        int   k;
    };
    struct CMD_ffff
    {
        float x;
        float y;
        float w;
        float h;
    };
    struct CMD_ffffi
    {
        float x;
        float y;
        float w;
        float h;
        int i;
    };
    struct CMD_ffffiii
    {
        float x;
        float y;
        float w;
        float h;
        int i;
        int j;
        int k;
    };
    struct CMD_ffffiif
    {
        float x;
        float y;
        float w;
        float h;
        int i;
        int j;
        float k;
    }; 
    struct CMD_fffffiiifi
    {
        float x;
        float y;
        float r;
        float s;
        float e;
        int f;
        int l;
        int m;
        float w;
        int v;
    };
    struct CMD_fffiiifi
    {
        float x;
        float y;
        float r;
        int f;
        int l;
        int m;
        float w;
        int v;
    };
    struct CMD_ffffiiif
    {
        float x;
        float y;
        float w;
        float h;
        int i;
        int j;
        int m;
        float k;
    };
    struct CMD_fffiifi
    {
        float x;
        float y;
        float r;
        int i;
        int j;
        float k;
        int l;
    };
    struct CMD_fffff
    {
        float x;
        float y;
        float w;
        float h;
        float i;
    };
    struct CMD_ffffifi
    {
        float fx;
        float fy;
        float tx;
        float ty;
        int c;
        float w;
        int i;
    };
    struct CMD_fffffi
    {
        float x;
        float y;
        float w;
        float h;
        float i;
        int j;
    };
    struct CMD_ffffff
    {
        float a;
        float b;
        float c;
        float d;
        float tx;
        float ty;
    };
    struct CMDMatrix4x4
    {
        float matrix[16];
    };
    struct CMDIBVB
    {
        int ibId;
        int vbId;
        int vertType;
        int shaderId;
        int imgId;
        int startIndex;
        int offset;
        int numElement;
        float x;
        float y;
        float mat[6];
    };
    struct CMDSkinMesh
    {
        int ibId;
        int vbId;
        int vertType;
        int numElement;
        int startIndex;
        int imgId;
        int shaderId;
        float mat[6];
        float x;
        float y;
    };
}
