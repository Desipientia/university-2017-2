<template>
  <input v-model="value"
         v-bind:readonly="isReadonly"
         v-on:focus="onfocus($event)"
         v-on:blur="onblur"
  />
</template>

<script>
  import Inputmask from 'inputmask';
  
  export default {
    name: 'SmartInput',
    computed: {
      value: {
        get () {
          return this.syncValue;
        },
        set (value) {
          this.$emit('update:syncValue', value);
        }
      }
    },
    props: {
      syncValue: [Number, String],
      type: String,
      isReadonly: {
        type: Boolean,
        default: false
      }
    },
    methods: {
      onfocus (event) {
        if (this.isReadonly) { return; }
        if (this.value === 0 && this.type === 'number') {
          this.value = '';
        } else {
          window.setTimeout(function() {
            event.target.select();
          }, 1);
        }
      },
      onblur () {
        if (this.value === '' && this.type === 'number') {
          this.value = 0;
        }
      },
    },
    mounted: function () {
      if (!this.isReadonly && this.type === 'number') {
        Inputmask('9', { repeat: 5, greedy: false }).mask($(this.$el));
      }
    }
  }
</script>