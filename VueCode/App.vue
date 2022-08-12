<template>
  <div>
    <el-menu
        :default-active="activeIndex"
        class="el-menu-demo"
        mode="horizontal"
        router
    >
      <el-menu-item index="/WaterPump">数字孪生可视化</el-menu-item>
      <el-menu-item index="/SQL">数据可视化</el-menu-item>
    </el-menu>
    <div style="margin-top:20px"> </div>
    <router-view> </router-view>
  </div>

</template>

<script>

export default {
  data(){
    return {
      activeIndex : '/WaterPump'
    }
  },
  methods: {
    handleSelect() {
      // 路由跳转时，标记在导航的那个选项下对应的路由
      this.activeIndex = this.$route.path
    },
  },
  watch:{
    '$route.path':{
      handler(){
        this.handleSelect()
      }
    }
  }
}

</script>
