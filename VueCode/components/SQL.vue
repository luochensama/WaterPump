<template>
  <el-input
      v-model="search"
      placeholder="搜索"
      :prefix-icon="Search"
      style="width:200px;">
    <template #append>
      <el-button :icon="Search"
       @click="searchData"/>
    </template>

  </el-input>

  <el-table :data="tableData.list">
    <el-table-column property="system" label="系统" width="150" />
    <el-table-column property="type" label="参数类型" width="200" >
      -----
    </el-table-column>
    <el-table-column property="id" label="参数ID" />
    <el-table-column property="name" label="参数名称" />
    <el-table-column property="operation" label="操作">
      <template v-slot="scope">
        <el-button text size="small" @click="handleClick(scope.row)"> 数据可视化 </el-button>
      </template>
    </el-table-column>
  </el-table>

  <el-drawer
      v-model="drawer"
      :title=title
      direction="btt"
      size="50%"
      @opened="opens">
    <div
        ref="echarts"
        :style="{ width: '100%', height: '100%' }">
    </div>


  </el-drawer>


</template>

<script>
//  import { getData } from '@/requests'
  import { Search } from '@element-plus/icons-vue';
  import Axios  from 'axios';
  import { reactive} from "vue";

  Axios.defaults.baseURL = '/api';

  export default {
    data(){
      return{
        Search,
        search:"",
        drawer:false,
        title:"",
      };
    },

    methods:{
      searchData(){ // 键盘事件
        this.getCurrentTable(this.search);
      },
      handleClick(row){
        this.drawer = true;
        this.title = "系统名称： " + row.system + " 参数ID： "+ row.id + " 参数名称: " + row.name
        this.getChartsData(row.id)
      },
      opens(){
        this.$echarts.init(this.$refs.echarts).setOption({
          xAxis: {
            type: 'category',
            data: this.echartsData.timeList,
          },
          yAxis: {
            type: 'value',
          },
          series: {
            data: this.echartsData.dataList,
            type: 'line'
          }
        })
      }
    },

    setup(){

      const tableData = reactive({
        list:""
      });
      const echartsData = reactive({
        timeList:"",
        dataList:""
      });
      Axios({
        method:'get',
        url:'/checkPoint.json',
        params: {
          search : ""
        }
      }).then((resp) => {
        tableData.list = resp.data;
      });
      const getCurrentTable = function (search){
        Axios({
          method:'get',
          url:'/checkPoint.json',
          params: {
            search : search,
          }
        }).then((resp) => {
          tableData.list = resp.data;
        });
      }
      const getChartsData = function(id){
        Axios({
          method:'get',
          url:'/data.json',
          params: {
            device_id : id,
          }
        }).then((resp) => {
          echartsData.dataList = resp.data["dataList"];
          echartsData.timeList = resp.data["timeList"];
        });
      }
      return {
        tableData,
        echartsData,
        getCurrentTable,
        getChartsData
      }
    },

    created() { // 页面创建的时候触发该函数
      this.search = this.$route.query.name;
      if(this.search === undefined){ // 默认
        this.search = "";
      }
      this.getCurrentTable(this.search);
    },
  }
</script>