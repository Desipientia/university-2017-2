import Vue from 'vue';

import VueResource from 'vue-resource';

import App from './App.vue';

Vue.use(VueResource);

Vue.http.options.root = 'dist/assets/';

new Vue({
  components: { App },
  render: h => h(App),
}).$mount('#app');
