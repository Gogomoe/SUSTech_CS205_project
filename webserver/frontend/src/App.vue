<template>
    <div id="app">
        <a-layout id="components-layout-demo-top" class="layout">
            <a-layout-header>
                <div class="logo"/>
                <a-menu
                        theme="dark"
                        mode="horizontal"
                        :default-selected-keys="['1']"
                        :style="{ lineHeight: '64px' }"
                        :selectedKeys="selectKey"
                >
                    <a-menu-item key="1">
                        <router-link to="/">Matrix</router-link>
                    </a-menu-item>
                    <a-menu-item key="2">
                        <router-link to="/image">Image</router-link>
                    </a-menu-item>
                </a-menu>
            </a-layout-header>

            <a-layout-content :style="{ display: 'flex', 'flex-direction': 'column', padding: '0 50px'}">
                <a-breadcrumb style="margin: 16px 0">
                    <a-breadcrumb-item>Home</a-breadcrumb-item>
                    <a-breadcrumb-item>Matrix</a-breadcrumb-item>
                </a-breadcrumb>

                <router-view
                        :style="{ background: '#fff', padding: '24px', minHeight: '280px', 'flex-grow': '1' }"></router-view>

            </a-layout-content>

            <a-layout-footer style="text-align: center">
                Matrix ©2020
            </a-layout-footer>
        </a-layout>
    </div>
</template>


<script>
    export default {
        computed: {
            selectKey: function () {
                let nameMapper = {
                    Home: "1",
                    Image: "2"
                }
                return [nameMapper[this.$route.name]];
            }
        },
    }
</script>

<style lang="scss">
    #app, #components-layout-demo-top {
        min-height: 100vh;
    }

    #components-layout-demo-top .logo {
        width: 120px;
        height: 31px;
        background: rgba(255, 255, 255, 0.2);
        margin: 16px 24px 16px 0;
        float: left;
    }
</style>
