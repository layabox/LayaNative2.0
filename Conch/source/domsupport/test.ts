
class JCMemClass{
    getChangedFlag():boolean{
        return false;
    }
    setChangedFlag(b:boolean){

    }
    getDataSize():number{
        return 0;
    }
    getData():ArrayBuffer{
        return null;
    }
}

/**
 * 一个方便操作的buffer
 * 1. 直接根据数据构建
 * 2. 随时修改：修改，添加,例如动态mesh
 * 3. 可以分段提交
 */

class JCGpuBuffer {
    //m_bChanged: boolean = false;  //任意段发生修改
    m_nStreamNum: number = 0; //
    vbo: WebGLBuffer = 0;//gpu res
    m_nVBLength: number = 0;//对应的显存的大小，如果内存大于显存了，需要重新调整
    m_pMemBuffer:JCMemClass[];
    m_vStreamStart: number[] = null;
    m_nTotalSize=0;
    create(size) {

    }
    /**
     * 分段管理
     */
    createStreams(s1size, s2size, s3size, ...args) {
        this.m_nTotalSize=0;//++++
    }

    /**
     * @param newvb 是否强制更新vb，不再使用老的。
     */
    uploadTo(target, newvb: boolean) {
        var gl:WebGLRenderingContext;
        //计算总大小
        this.m_nTotalSize=0;
        for(var i=0; i<this.m_pMemBuffer.length; i++){
           this.m_nTotalSize+= this.m_pMemBuffer[i].getDataSize();
        }    

        if( this.vbo>0 && (newvb || this.m_nTotalSize>this.m_nVBLength)){
            gl.deleteBuffer(this.vbo);//TODO 用重用的方法更好
            this.vbo=0;
        }

        if(this.vbo<=0){
            this.vbo = gl.createBuffer();
            gl.bindBuffer(target,this.vbo);
            gl.bufferData(target,this.m_nTotalSize,gl.DYNAMIC_DRAW);   //分配空间
        }
        gl.bindBuffer(target,this.vbo);
        for( var i=0; i<this.m_nStreamNum; i++){
            gl.bufferSubData(target,this.m_vStreamStart[i],this.m_pMemBuffer[i].getData());
            this.m_pMemBuffer[i].setChangedFlag(false);
        }
        this.m_nVBLength=this.m_nTotalSize;
    }

    getStreamMem(idx) {

    }
    freeGLRes() {

    }
}


class JCVB extends JCGpuBuffer {
    upload(force) {

    }
}

class JCIB extends JCGpuBuffer {
    createFromArray(arr) {

    }
    upload(force) {

    }
    appendData(p_pIdx, p_nNum, p_nVertOff){

    }
}

class JCNamedData {

}

class JCVertexDesc {
    m_VertDescs:any[];
    getStride(): number {
        return 0;
    }
    getHash() {
    }
}

class JCShaderLinkInfo{
    //这里包含stride
}

class JCMaterial {

}


/**
 * 类似submesh
 */
class JCRenderGroupData {
    m_pMesh: JCMesh;
    m_nGeoMode: number;
    m_nBegin: number = 0;  //可能是顶点位置也可能是index位置
    m_nEnd: number = 0;
    m_bHasObjMat = false;
    m_pShaderInfo: any = null;
    m_pMaterial: JCMaterial = null;
    m_nVertexDesc = 0;    //第几个顶点描述
}

class JCMesh {
    //m_nVertNum;
    /**
     * 本mesh用到的所有的顶点格式
     */
    m_AllVertexDesc: JCVertexDesc[];
    getVB(): JCVB {
        return null;
    }
    getIB(): JCIB {
        return null;
    }

    //缺省大小不用设置
    //config(INITSize);
    // getStride(id): number {
    //     return this.m_AllVertexDesc[id].getStride();
    // }

    regVertexDesc(vd:JCVertexDesc):number{
        var h =vd.getHash();
        //if( )
        return this.m_AllVertexDesc.length;
    }
    freeGLRes() {

    }
    //删除内存数据，例如不会变的mesh
    freeMemRes() {

    }
    upload() {
    }
    getGroupNum():number{
        return 0;
    }
}

/**
 * 单流mesh
 * TODO 是否需要一个只保留gpu资源的mesh
 */
class JCMesh1 extends JCMesh{
    m_pCurGroup:JCRenderGroupData=null;
    m_vRenderGroupDatas:JCRenderGroupData[]=null;
    //直接创建vertex
    pushVertex(vertType:number, geomode, mtl, {}):JCRenderGroupData {
        return null;
    }
    /**
     * 这种情况geomode固定为三角形
     * @param relIdx 是否是相对的索引，如果是true，需要全部加上当前的顶点个数
     */
    pushElement(vertType:number, mtl, {}, {}, relIdx:boolean) {

    }

    pushVertexBuf(vertType:number, geomode, mtl, data, len) {
        var vetNum = 0;//datalen/stride
    }
    pushElementsBuf(vertType:number, mtl,vbdata, vblen, ibdata, iblen, relIdx:boolean):JCRenderGroupData {
        if(vertType!=this.m_pCurGroup.m_nVertexDesc ||true){
            //new group
        }
        return this.m_pCurGroup;
    }
}

function renderMesh(mesh: JCMesh, group: JCRenderGroupData, data, datanum) {
    var stride = group.getStride();

}

function fillTris(ib, vb, vertexdesc, shader, texture) {

}

class JCHtml5Context {
    m_pDynamicMesh:JCGameMesh=null;
    setIBVB(p_nIbId, p_nVbId, p_nShaderId, p_nImgId,
        p_nStartIndex, p_nOffset, p_nNumElement, x, y, vertextype) {

        var vbbuf;
        var ibbuf;
        var gpuProg;
        var Img;
        switch (vertextype) {
            case 1://x,y,u,v,r,g,b,a
            case 2://x,y,u,v
        }
        var vdesc: JCVertexDesc = null;   //在ctx的成员变量中写死
        var mtl:JCMaterial;
        if(!vbbuf.vdid){//or <0
            vbbuf.viid=this.m_pDynamicMesh.regVertexDesc(vdesc);
        }
        this.m_pDynamicMesh.pushElementsBuf(vbbuf.viid,mtl,vbbuf,1,ibbuf,1,true);

    }

    pushTriangleStripsData(pData, len,mtl){

    }
}


//动态添加
//材质
//矩阵
//link需要全局的数据的描述
//link结果保存在什么地方
class JCGameMesh extends JCMesh1 {
    strokeLink: JCShaderLinkInfo;
    fillLink: JCShaderLinkInfo;
    mapLink: JCShaderLinkInfo;
    fillcolorLink: JCShaderLinkInfo;
    drawImgLink: JCShaderLinkInfo;
    render() {

    }
}

//JCVertexDesc 和stride其实并没有什么关联 mesh属性描述中增加 m_nStride
//JCVertexDesc 与attrib有关

class JCRenderCmd{

}

function testWS(){
    var ws = new WebSocket('ws://174.129.224.73/');
    ws.onopen=function(ev){
        console.log('open');
        ws.send('hello');
    }
    ws.onmessage=function(msg){
        console.log(msg);
    }
}