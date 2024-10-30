import { Fragment, useState, useLayoutEffect } from "react"
import { FaCheckCircle } from "react-icons/fa"
import { Button } from "react-bootstrap"


const SettingsComponent = (props) => {
    const [ipAddress, setIPAddress] = useState('')

    useLayoutEffect(() => {
        const ip = localStorage.getItem('esp_ip')
        if (ip === "" || ip === null) {
            alert("Please configure ESP32 IP address.")
            setIPAddress('192.168.43.127')
        } else {
            setIPAddress(ip)
        }
    }, [])

    const handleChangeIP = () => {
        if (validateIP(ipAddress)) {
            localStorage.setItem('esp_ip', ipAddress)
            alert("IP address has been set!")
            props.onParamChange()
        } else {
            alert("Incorrect IP address!")
        }
    }

    const validateIP = (input) => {
        const ipRegex = /\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.|$)){4}\b/
        return ipRegex.test(input)
    }

    return (
        <Fragment>
            <div className="workspace-centered">
                <div className="box-row">
                    <div className="box-parameters">
                        <div className="box-col">
                            <div className="box-text">
                                Settings
                            </div>
                        </div>
                    </div>
                </div>
                <div className="row-many-buttons">
                    <div className="input-text">
                        ESP-32 IP address:
                        <div>
                            <input type="text" value={ipAddress} onChange={(event) => setIPAddress(event.target.value)} />
                        </div>
                    </div>
                    <Button onClick={handleChangeIP}>
                        <FaCheckCircle size={30} />
                    </Button>
                </div>
            </div>
        </Fragment>
    );
};

export default SettingsComponent;