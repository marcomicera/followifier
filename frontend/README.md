# `frontend` module

## Dependencies
1. Install Node.js and npm:
    ```bash
    $ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash - && sudo apt install -y nodejs
    ```
1. Install [`reader`](reader)'s and [`interface`](interface)'s dependencies:
    ```bash
    $ npm install reader
    $ npm install interface
    ```

## Launch the `frontend` server
1. Launch the [`reader`](reader) component:
    ```bash
    $ node reader/index.js
    ```
1. Open a new shell and start the GUI:
    ```bash
    $ npm start --prefix interface
    ```
1. Open the GUI at [http://localhost:4200/](http://localhost:4200)
