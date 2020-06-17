<template>
    <div>
        <a-upload
                list-type="picture"
                :file-list="fileList"
                :remove="handleRemove"
                :before-upload="beforeUpload"
        >
            <div v-if="fileList.length === 0">
                <a-button>
                    <a-icon type="upload"/>
                    upload
                </a-button>
            </div>
        </a-upload>

        <a-row :gutter="24" v-if="fileList[0] && fileList[0].status === 'done'" style="padding: 0.5em 0">
            <a-col :span="4" style="line-height: 32px">
                kernel size
            </a-col>
            <a-col :span="12" style="line-height: 32px">
                <a-slider v-model="convolveKernelSize" :min="1" :max="50" style="width: 100%"/>
            </a-col>
            <a-col :span="4" style="line-height: 32px">
                <a-input-number v-model="convolveKernelSize" :min="1" :max="50" style="width: 100%"/>
            </a-col>
            <a-col :span="4" style="line-height: 32px">
                <a-button v-on:click="convolve" style="width: 100%">convolve</a-button>
            </a-col>
        </a-row>
    </div>
</template>

<script>

    export default {
        name: "ImageInput",
        data() {
            return {
                fileList: [],
                convolveKernelSize: 10
            };
        },
        methods: {
            convolve: async function () {
                let img = this.readImage();
                let size = this.convolveKernelSize;

                let kernel = Array.from(new Array(size).keys()).map(rows =>
                    Array.from(new Array(size).keys()).map(it =>
                        1.0 / (size * size)
                    )
                );
                let data = {
                    kernel: {
                        rows: size,
                        cols: size,
                        data: kernel
                    }
                }

                let formData = new FormData();
                formData.append("img1", img.origin);
                formData.append("json", JSON.stringify(data));

                let resp = await fetch("/api/image/convolve", {method: "POST", body: formData});
                if (!resp.ok) {
                    this.$message.error('Error: operation failed');
                    return;
                }
                let arrayBuffer = await resp.arrayBuffer();
                let result = 'data:image/jpeg;base64,' + btoa(
                    new Uint8Array(arrayBuffer)
                        .reduce((data, byte) => data + String.fromCharCode(byte), '')
                );
                this.$emit('update:result', result);
            },
            handleRemove() {
                this.fileList = [];
            },
            beforeUpload(file) {
                let fileItem = {
                    uid: `x${file.uid}`,
                    name: file.name,
                    status: "uploading",
                    origin: file
                };
                this.fileList = [fileItem];

                let reader = new FileReader();
                reader.onload = function () {
                    fileItem.url = 'data:image/jpeg;base64,' + btoa(
                        new Uint8Array(reader.result)
                            .reduce((data, byte) => data + String.fromCharCode(byte), '')
                    );

                    fileItem.bytes = reader.result;

                    let img = new Image();
                    img.src = fileItem.url;
                    img.onload = function () {
                        fileItem.width = this.width;
                        fileItem.height = this.height;
                        fileItem.status = "done";
                    }
                }
                reader.readAsArrayBuffer(file);

                return false;
            },
            readImage() {
                if (this.fileList.length === 0 || this.fileList[0].status !== "done") {
                    return null;
                }
                return this.fileList[0];
            }
        },
    }
</script>

<style scoped>

</style>