<template>
    <div>
        <h1>Image Calculate</h1>
        <a-row>
            <a-col :span="10">
                <h3>Image A</h3>
                <ImageInput ref="imageA" v-on:update:result="result = $event"></ImageInput>
            </a-col>
            <a-col :span="4">
                <div class="operator-panel">
                    <a-button v-on:click="exchange">↔</a-button>
                    <a-button v-on:click="plus">A + B</a-button>
                    <a-button v-on:click="minus">A - B</a-button>
                </div>
            </a-col>
            <a-col :span="10">
                <h3>Image B</h3>
                <ImageInput ref="imageB" v-on:update:result="result = $event"></ImageInput>
            </a-col>
        </a-row>
        <template v-if="result">
            <h3>Result:</h3>
            <img v-bind:src="result" alt="result">
        </template>
    </div>
</template>

<script>
    import ImageInput from "../components/ImageInput.vue";

    function binaryOperate(api) {
        return async function () {
            let imgA = this.$refs.imageA.readImage();
            let imgB = this.$refs.imageB.readImage();

            if (!imgA || !imgB) {
                this.$message.error('Error: image not select');
                return;
            }

            let formData = new FormData();
            formData.append("img1", imgA.origin);
            formData.append("img2", imgB.origin);

            let resp = await fetch(api, {method: "POST", body: formData});
            if (!resp.ok) {
                this.$message.error('Error: operation failed');
                return;
            }
            let arrayBuffer = await resp.arrayBuffer();
            this.result = 'data:image/jpeg;base64,' + btoa(
                new Uint8Array(arrayBuffer)
                    .reduce((data, byte) => data + String.fromCharCode(byte), '')
            );
        }
    }

    export default {
        name: "ImagePage",
        data: function () {
            return {
                result: ''
            }
        },
        components: {
            ImageInput
        },
        methods: {
            plus: binaryOperate("/api/image/plus"),
            minus: binaryOperate("/api/image/minus"),
            exchange: function () {
                let imgA = this.$refs.imageA.readImage();
                let imgB = this.$refs.imageB.readImage();
                this.$refs.imageA.fileList = imgB ? [imgB] : [];
                this.$refs.imageB.fileList = imgA ? [imgA] : [];
            }
        }
    }
</script>

<style scoped>
    .operator-panel {
        width: 100%;
        height: 100%;
        display: flex;
        flex-direction: column;
        box-sizing: border-box;
        padding: 6em 2em 0;
    }
</style>