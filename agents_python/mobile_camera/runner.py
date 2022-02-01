from .app import app

import pathlib
template_path = pathlib.Path(__file__).parent.resolve().joinpath('templates')
app.template_folder = str(template_path)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=app.config['DEBUG'])
    pass
