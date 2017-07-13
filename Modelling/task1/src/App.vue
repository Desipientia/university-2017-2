<template>
  <div class="container">
    <div class="input-area">
      <label for="file-input">
        <span class="caption">Изображение</span>
        <input id="file-input"
               type="file"
               accept="image/*"
               v-on:change="loadImage($event)"/>
      </label>
      <div class="controls"
           v-show="isImageLoaded">
        <button class="button" v-on:click="">
          Снять выделение
        </button>
        <button class="button" v-on:click="">
          Сформировать полигон
        </button>
        <div class="label">
          <label class="settings">
            <span class="caption">Масштаб</span>
            <smart-input v-bind:syncValue.sync="scale"
                         type="number">
            </smart-input>
            <span class="suffix">м в 1px</span>
          </label>
        </div>
        <div class="label">
          <label>
            <span class="caption">Размер ущерба на 1 кв. м</span>
            <smart-input v-bind:syncValue.sync="cost"
                         type="number">
            </smart-input>
            <span class="suffix">у. е.</span>
          </label>
        </div>
        <div class="result">
          <p>Площадь равна &nbsp;{{ area }}&nbsp; кв. м</p>
          <p>Нанесённый ущерб равен &nbsp;{{ loss }}&nbsp; у. е</p>
        </div>
      </div>
    </div>
    <div id="image" class="image"></div>
  </div>
</template>

<script>
  import SmartInput from './components/SmartInput.vue';
  
  export default {
    name: 'App',
    data: function () {
      return {
        isImageLoaded: false,
        imageLink: '',
        image: null,
        context: null,
        scale: 0,
        cost: 0
      }
    },
    computed: {
      area () {
        return 0;
      },
      loss () {
        return 0;
      }
    },
    components: { SmartInput },
    methods: {
      loadImage (e) {
        let file = e.target.files[0];
        
        this.imageLink = file.name;
        this.$http.get(this.imageLink, { responseType: 'arraybuffer' }).then(response => {
          this.image = (new Tiff({ buffer: response.body })).toCanvas();
          this.isImageLoaded = true;
          this.context = this.image.getContext('2d');
          $('#image').append($(this.image));
        }, response => {
          console.log(response);
        });
      }
    },
    mounted: function () {
      Tiff.initialize({
        TOTAL_MEMORY: 1000000000
      });
    }
  };
</script>

<style lang="scss">
  body {
    margin: 0;
    width: 100%;
    font-family: Helvetica, Arial, sans-serif;
    
    .input-area {
      position: absolute;
      top: 0;
      right: 0;
      display: flex;
      flex-direction: column;
      justify-content: space-between;
      width: 300px;
      margin: 10px;
      padding: 20px;
      border-radius: 4px;
      border: solid 1px black;
      background-color: aliceblue;
      
      input,
      button {
        padding: 8px;
        border-radius: 4px;
        border: solid 1px black;
        font-size: 14px;
      }
      button {
        width: 100%;
        background-color: lightsteelblue;
        cursor: pointer;
      }
      input {
        width: 280px;
        margin: 3px 0;
      }
      .controls {
        width: 300px;
        margin-top: 10px;
        padding-top: 10px;
        border-top: dashed 1px black;
        
        button {
          width: 100%;
          margin: 5px 0;
        }
        input {
          width: 100px;
          padding: 5px;
        }
        .label {
          margin: 10px 0;
        }
      }
      .result {
        width: 300px;
        margin-top: 10px;
        padding-top: 10px;
        border-top: dashed 1px black;
      }
    }
  }
</style>
