import React, {Component} from 'react';
// Components
import { Button } from 'components';

class Login extends Component {

    constructor(props) {
        // Inherit constructor
        super(props);
        // State for form data and error message
        this.state = {
            form: {
            username: '',
            key: '',
            },
        }

        //Bind functions
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleChange(event) {
        const { name, value } = event.target;
        const { form } = this.state;
    
        this.setState({
          form: {
            ...form,
            [name]: value,
          },
        });
    }

    handleSubmit(event) {
        //Suppress the default submit behavior
        event.preventDefault();

        //TODO: submit credentials to the blockchain
    }

    render() {
        //Extracted some data from the state
        const { form } = this.state;
        return (
            <div className="Login">
            <div className="title">Elemental Battles - powered by EOSIO</div>
            <div className="description">Please use the Account Name and Private Key generated in the previous page to log into the game.</div>
            <form name="form">
              <div className="field">
                <label>Account name</label>
                <input
                  type="text"
                  name="username"
                  placeholder="All small letters, a-z, 1-5 or dot, max 12 characters"
                  pattern="[\.a-z1-5]{2,12}"
                  required
                />
              </div>
              <div className="field">
                <label>Private key</label>
                <input
                  type="password"
                  name="key"
                  pattern="^.{51,}$"
                  required
                />
              </div>
              <div className="bottom">
                <Button type="submit" className="green">
                  { "CONFIRM" }
                </Button>
              </div>
            </form>
          </div>
        )
    }
}

export default Login;