import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import * as echarts from 'echarts'
import App from './App.vue'
import {createRouter, createWebHistory} from 'vue-router'
import WaterPump from "@/components/WaterPump";
import SQL from "@/components/SQL";

let router
const app = createApp(App)
app.config.globalProperties.$echarts = echarts;
app.use(ElementPlus)
setupRouter();
app.use(router)
app.mount('#app')

function setupRouter(){
    const routes = [
        {
            path: '/',
            redirect:'/WaterPump',//设置路由重定向第一次进入的页面
        },
        {
            path: '/WaterPump',
            name: 'WaterPump',
            component: WaterPump
        },
        {
            path: '/SQL',
            name: 'SQL',
            component: SQL
        }
    ];
    router = createRouter({
        history: createWebHistory(),
        routes,
    })
}
export default router