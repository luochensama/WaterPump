<template>

  <el-drawer
      v-model="drawer"
      :title=title
      direction="rtl"
      size="50%">
    <el-table :data="gridData">

      <el-table-column property="id" label="序号" width="150" />
      <el-table-column property="point" label="测点位号" width="200" />
      <el-table-column property="name" label="被测参数名称" />
      <el-table-column property="operation" label="操作">
        <template v-slot="scope">
          <el-button text size="small" @click="handleClick(scope.row)"> 详情 </el-button>
        </template>
      </el-table-column>

    </el-table>
  </el-drawer>

  <el-row :gutter="0">
    <el-col :span="4">
      <el-cascader :options="setupData.options" :show-all-levels="false" @change="bindComponents" ref="selectItem"/>
    </el-col>
    <el-col :span="4">
       <el-button color="#ff0000" style="width: 80%;height: 100%" @click="changeColor(this.red)"> 红色 </el-button>
    </el-col>
    <el-col :span="4">
      <el-button  color="#0000ff" style="width: 80%;height: 100%" @click="changeColor(this.blue)"> 蓝色 </el-button>
    </el-col>
    <el-col :span="4">
      <el-button  color="#ffff00" style="width: 80%;height: 100%" @click="changeColor(this.yellow)"> 黄色 </el-button>
    </el-col>
    <el-col :span="4">
      <el-button  color="#8B8989" style="width: 80%;height: 100%" @click="changeColor(this.reset)"> 重置 </el-button>
    </el-col>
  </el-row>

  <div id="container"></div>

</template>
<style>
.el-row {
  margin-bottom: 20px;
}
.el-row:last-child {
  margin-bottom: 0;
}
.el-col {
  border-radius: 4px;
}

.grid-content {
  border-radius: 4px;
  min-height: 36px;
}
</style>

<script>
import * as THREE from 'three';
import {OrbitControls} from "../libs/three/OrbitControls";
import {GLTFLoader} from '../libs/three/GLTFLoader';
import {reactive} from "vue";
import Axios from "axios";
Axios.defaults.baseURL = '/api';

let scene,group,components = {};

export default {
  setup(){
    const setupData = reactive({
      options:"",
      json:"",
    });
    Axios({
      method:'get',
      url:'/WaterPumpList.json',
    }).then((resp) => {
      setupData.json = resp.data;
      let list = [];
      for(let key in setupData.json){
        let temp = {};
        temp["value"] = setupData.json[key]["name"];
        temp["label"] = setupData.json[key]["name"];
        list.push(temp);
      }
      setupData.options = list;
    })
    return {
      setupData
    }
  },
  data() {
    return {
      camera: '', //创建相机
      renderer: '', //创建渲染器
      glbObject: '', //创建展示对象
      container:'',
      title:'',
      drawer: false,
      gridData: '',
      controls: '',
      select:'',
      red:0xff0000,
      blue:0x0000ff,
      yellow:0xffff00,
      reset:0x8B8989,
      selectObject: "", //与射线相交的数组
      requestID: '',// 作为启用动画时返回得id，用于路由跳转离开页面得时候停止动画
    };
  },
  mounted() {
    this.init();
    window.addEventListener('resize', this.onWindowResize, false);
    window.addEventListener('click', this.onMouseDblclick, false);
  },
  beforeUnmount() {
    window.removeEventListener('resize', this.onWindowResize);
    window.removeEventListener('click', this.onMouseDblclick);
    // 页面离开时停止动画
    window.cancelAnimationFrame(this.requestID);
  },
  methods : {
    init() {
      scene = new THREE.Scene();
      group = new THREE.Group();
      scene.background = new THREE.Color(0x333333);
      let light = new THREE.DirectionalLight(0xffffff, 4);
      light.position.setScalar(100);
      scene.add(light);
      scene.add(new THREE.AmbientLight(0xffffff, 0.5));
      // 创建相机
      this.camera = new THREE.PerspectiveCamera(15, window.innerWidth / window.innerHeight, 0.1, 1000);
      this.camera.position.set(40, 10, 0); // 设置相机位置
      this.camera.lookAt(0,0,0);
      this.camera.up.set(0, 0, 1); // 设置旋转
      this.initJson();
      this.createObj();
      this.createRender();
      this.createControls();
      this.animate();
    },
    changeColor(color){
      let tempList = components[this.select];
      for(let i = 0;i < tempList.length; ++ i){
        tempList[i].material.color.set(color);
      }
    },
    bindComponents(){
      this.select = this.$refs["selectItem"].getCheckedNodes()[0].label.slice(0,7);
    },
    createObj() {
      let texture = new THREE.TextureLoader().load("webp.jpg");
      const loader = new GLTFLoader().setPath('models/');
      loader.load('WaterPump.glb',function (glb) {
        glb.scene.scale.set(0.15, 0.15, 0.15);
        glb.scene.position.set(0, 0, -0.8);
        // glb.scene.rotateY(-Math.PI/8);
        glb.scene.traverse(function (child){
          if ( child instanceof THREE.Mesh ) {
            if(child.parent.name.indexOf("PRT") == 0) {
              child.material = new THREE.MeshPhongMaterial({map:texture,color:0x8B8989});
              // eslint-disable-next-line no-prototype-builtins
              if(components.hasOwnProperty(child.parent.name) === false){
                components[child.parent.name] = [];
              }
              components[child.parent.name].push(child);
            }
          }
        });

        group.add(glb.scene);
        scene.add(group);
      })
    },
    //创建渲染器
    createRender() {
      this.renderer = new THREE.WebGLRenderer();
      this.renderer.setClearColor(0x404040)
      this.renderer.setSize(window.innerWidth, window.innerHeight);
      this.container = document.getElementById('container');
      this.container.innerHTML = "";
      this.container.appendChild(this.renderer.domElement);
    },
    createControls(){
      this.controls = new OrbitControls(this.camera, this.renderer.domElement);
      this.controls.addEventListener('change', this.render); // use if there is no animation loop
      this.controls.minDistance = 2;
      this.controls.maxDistance = 10;
      this.controls.target.set(0, 0, -0.2);

      this.controls.update();
      this.controls.enabled = false;
    },
    // 渲染场景
    animate() {
      this.requestID = requestAnimationFrame(this.animate);
      // this.controls.update();
      this.render();
    },
    // 设置物体行为
    render() {
      this.renderer.render(scene, this.camera);
    },
    // 窗口变动触发的方法
    onWindowResize() {
      this.camera.aspect = window.innerWidth / window.innerHeight;
      this.camera.updateProjectionMatrix();
      this.renderer.setSize(window.innerWidth, window.innerHeight);
    },
    // 鼠标双击触发的方法
    onMouseDblclick(event) {
      // 获取 raycaster 和所有模型相交的数组，其中的元素按照距离排序，越近的越靠前
      let intersects = this.getIntersects(event);
      // 获取选中最近的 Mesh 对象
      if (intersects.length !== 0 && intersects[0].object.type === 'Mesh') {
        this.selectObject = intersects[0].object.parent;
        let node = this.setupData.json[this.selectObject.name]
        this.gridData = node['point'];
        this.title = node.name;
        this.drawer = true;
      } else {
        console.log('未选中 Mesh!');
      }
    },
    // 获取与射线相交的对象数组
    getIntersects(event) {
      event.preventDefault();
      // 声明 raycaster 和 mouse 变量
      let raycaster = new THREE.Raycaster();
      let mouse = new THREE.Vector2();

      // 通过鼠标点击位置,计算出 raycaster 所需点的位置,以屏幕为中心点,范围 -1 到 1
      mouse.x = ( (event.clientX - this.container.getBoundingClientRect().left) / this.container.offsetWidth) * 2 - 1;
      mouse.y = -((event.clientY - this.container.getBoundingClientRect().top) / this.container.offsetHeight) * 2 + 1;

      //通过鼠标点击的位置(二维坐标)和当前相机的矩阵计算出射线位置
      raycaster.setFromCamera(mouse, this.camera);

      // 获取与射线相交的对象数组，其中的元素按照距离排序，越近的越靠前
      let intersects = raycaster.intersectObjects(group.children);

      //返回选中的对象
      return intersects;
    },
    initJson(){

    },
    handleClick(row){
      console.log(row.name);
      this.$router.push({
        path : '/SQL',
        query:{
          name:row.point
        }
      });
    }
  }
}


</script>