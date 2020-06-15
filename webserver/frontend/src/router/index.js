import Vue from "vue";
import VueRouter from "vue-router";
import Home from "../views/Matrix.vue";
import Image from "../views/Image.vue";

Vue.use(VueRouter);

const routes = [
  {
    path: "/",
    name: "Home",
    component: Home
  },
  {
    path: "/image",
    name: "Image",
    component: Image
  }
];

const router = new VueRouter({
  routes
});

export default router;
