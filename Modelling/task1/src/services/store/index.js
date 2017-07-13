import Vue from 'vue';

export default {
  state: {
    token: null,
  },

  getters: {},

  mutations: {
    authorize(state, { token }) {
      setToken({ token });
      state.token = token;
    },
    logout(state) {
      removeToken();
      state.token = getToken();
    },
  },

  actions: {
    login({ commit }, { username, login }) {
      // Vue.http.post(...).then(response => {
      //   commit('authorize', response.body);
      // });
      commit('authorize', { token: 'token1' });
    }
  }
}