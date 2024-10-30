import React, { Fragment, useState, useEffect } from 'react'
import NavbarComponent from './components/NavbarComponent'
import "./style.css"
import HistoryComponent from './components/HistoryComponent'
import HomeComponent from './components/HomeComponent'
import SettingsComponent from './components/SettingsComponent'

function App() {
  const [selectedButton, setSelectedButton] = useState("")
  const [espIp, setEspIp] = useState("")
  const [param, setParam] = useState('');

  useEffect(() => {
    const ip = localStorage.getItem('esp_ip')
    if (ip === "" || ip === null) {
      setSelectedButton("settings")
    } else {
      setSelectedButton("home")
      setEspIp(ip)
    }
  }, [param])

  const handleNavbarButtonClick = (option) => {
    setSelectedButton(option)
  };

  const handleParamChange = () => {
    setParam(!param)
  };

  return (
    <Fragment>
      <div className='app'>
        <div className="sidebar-container">
          <NavbarComponent selectedButton={selectedButton} onButtonChange={handleNavbarButtonClick} />
        </div>
        <div className="work-area">
          {selectedButton === "home" ? <HomeComponent espIp={espIp} /> : null}
          {selectedButton === "history" ? <HistoryComponent espIp={espIp} /> : null}
          {selectedButton === "settings" ? <SettingsComponent onParamChange={handleParamChange} /> : null}
        </div>
      </div>
    </Fragment>
  );
}

export default App;
