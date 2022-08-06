class Things:

    def __init__(self, name, device_id):
        self.name = name
        self.state = ""
        self.device_id = device_id

    def get_state(self):
        pass

    def to_dict(self):
        return {"name": self.name, "value": self.get_state()}

